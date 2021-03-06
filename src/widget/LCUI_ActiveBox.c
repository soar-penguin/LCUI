/* ***************************************************************************
 * LCUI_ActiveBox.c -- ActiveBox widget, play simple dynamic picture
 * 
 * Copyright (C) 2012 by
 * Liu Chao
 * 
 * This file is part of the LCUI project, and may only be used, modified, and
 * distributed under the terms of the GPLv2.
 * 
 * (GPLv2 is abbreviation of GNU General Public License Version 2)
 * 
 * By continuing to use, modify, or distribute this file you indicate that you
 * have read the license and understand and accept it fully.
 *  
 * The LCUI project is distributed in the hope that it will be useful, but 
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY 
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GPL v2 for more details.
 * 
 * You should have received a copy of the GPLv2 along with this file. It is 
 * usually in the LICENSE.TXT file, If not, see <http://www.gnu.org/licenses/>.
 * ****************************************************************************/
 
/* ****************************************************************************
 * LCUI_ActiveBox.c -- ActiveBox部件, 播放简单的动态图像
 *
 * 版权所有 (C) 2012 归属于 
 * 刘超
 * 
 * 这个文件是LCUI项目的一部分，并且只可以根据GPLv2许可协议来使用、更改和发布。
 *
 * (GPLv2 是 GNU通用公共许可证第二版 的英文缩写)
 * 
 * 继续使用、修改或发布本文件，表明您已经阅读并完全理解和接受这个许可协议。
 * 
 * LCUI 项目是基于使用目的而加以散布的，但不负任何担保责任，甚至没有适销性或特
 * 定用途的隐含担保，详情请参照GPLv2许可协议。
 *
 * 您应已收到附随于本文件的GPLv2许可协议的副本，它通常在LICENSE.TXT文件中，如果
 * 没有，请查看：<http://www.gnu.org/licenses/>. 
 * ****************************************************************************/
//#define DEBUG
#include <LCUI_Build.h>
#include <unistd.h>
#include LC_LCUI_H
#include LC_WIDGET_H 
#include LC_MISC_H
#include LC_MEM_H
#include LC_GRAPH_H
#include LC_ACTIVEBOX_H
#include LC_RES_H

/*
 * 用了一个线程完成这些动画的每一帧的更新，还用了一个“库”，用于记录总动画信息；
 * 加了个队列，用于记录需要进行帧切换的动画，按照延迟时间从短到长排序，每一帧更新完后，
 * 将当前动画切换至下一帧，并重新排序。
 * */
 
/*********************** Frames Process *******************************/
/* 动画库，用于记录所有动画的信息 */
static LCUI_Queue frames_database;
/* 动画流，以流水线的形式处理每个动画的每帧图像的更新 */
static LCUI_Queue frames_stream;

static int database_init = FALSE;
static int __timer_id = -1;

LCUI_Frames* 
Create_Frames(LCUI_Size size)
/* 
 * 功能：创建一个能存放动画数据的容器
 * 说明：该容器用于记录动画的每一帧的信息，需要指定该容器的尺寸。
 *  */
{
	int pos;
	LCUI_Frames *p, frames; 
	 
	Queue_Init(&frames.pic, sizeof(LCUI_Frame), NULL);
	Queue_Init(&frames.func_data, sizeof(LCUI_Func), NULL);
	Graph_Init(&frames.slot);
	frames.slot.have_alpha = TRUE;
	frames.current = 0;
	frames.status = 0;
	frames.size = size; 
	if( !database_init ) {
		Queue_Init(&frames_database, sizeof(LCUI_Frames), NULL); 
		database_init = IS_TRUE;
	}
	/* 记录该动画至库中 */ 
	pos = Queue_Add(&frames_database, &frames); 
	p = Queue_Get(&frames_database, pos); 
	return p;
}

LCUI_Pos Frame_Get_Pos(LCUI_Frames *stream, LCUI_Frame *frame)
/* 功能：获取指定帧在整个动画容器中的位置 */
{
	LCUI_Pos pos;
	pos = Align_Get_Pos(	stream->size, 
				Get_Graph_Size(frame->pic), 
				ALIGN_MIDDLE_CENTER );
	return Pos_Add(pos, frame->offset);
}

int Resize_Frames(LCUI_Frames *p, LCUI_Size new_size)
/* 功能：调整动画的容器尺寸 */
{
	if(new_size.w <= 0 || new_size.h <= 0)	{
		return -1;
	}
	if( !p ) {
		return -2;
	}
	
	int i, total;
	LCUI_Pos pos;
	LCUI_Frame *frame;
	LCUI_Graph *graph;
	LCUI_Size size;
	
	p->size = new_size;
	total = Queue_Get_Total(&p->pic);
	for(i=0; i<total; ++i){
		frame = Queue_Get(&p->pic, i);
		graph = frame->pic->src;
		size = Get_Graph_Size(graph);
		pos = Frame_Get_Pos(p, frame);
		if(pos.x+size.w > new_size.w){
			size.w = new_size.w - pos.x;
			size.w<0 ? size.w=0 :1;
		}
		if(pos.y+size.h > new_size.h){
			size.h = new_size.h - pos.y;
			size.h<0 ? size.h=0 :1;
		}
		Quote_Graph(frame->pic, graph, Rect(0,0,size.w, size.h));
	}
	Graph_Create(&p->slot, new_size.w, new_size.h);
	return 0;
}

int Add_Frame(	LCUI_Frames *des, LCUI_Graph *pic, 
		LCUI_Pos offset, int sleep_time )
/* 
 * 功能：为动画添加帧 
 * 说明：
 * pic指的是该帧的图像；
 * pos代表该帧图像在整个动画画面中的位置；
 * sleep_time表示该帧的显示时长（单位：毫秒）
 * */
{
	if( !des ) {
		return -1;
	}
	if(!Graph_Valid(pic)) {
		return -2;
	}
	
	LCUI_Frame frame;
	frame.offset = offset;
	frame.sleep_time = sleep_time;
	frame.pic = pic;
	frame.current_time = frame.sleep_time;
	Queue_Add(&des->pic, &frame); 
	return 0;
}

int Frames_Add_Func(	LCUI_Frames *des, 
			void (*func)(LCUI_Graph*, void*), 
			void *arg )
/* 
 * 功能：为动画关联回调函数 
 * 说明：关联回调函数后，动画每更新一帧都会调用这个函数
 * */
{
	if( !des ) {
		return -1;
	}
	LCUI_App *app;
	app = Get_Self_AppPointer();
	if( !app ) {
		return -2;
	}
	
	LCUI_Func func_data;
	func_data.func = func;
	func_data.id = app->id;
	func_data.arg[0] = NULL;
	func_data.arg[1] = arg;
	/* 如果不将该标志置为FALSE，会是确定的值，导致在执行任务后的销毁参数时出现段错误 */
	func_data.destroy_arg[0] = FALSE;
	func_data.destroy_arg[1] = FALSE;
	Queue_Add(&des->func_data, &func_data);
	return 0;
} 

static void 
Frames_Stream_Sort()
/* 功能：对动画流进行排序 */
{
	int i, j, pos, total;
	LCUI_Frames *temp;
	LCUI_Frame *p, *q;
	
	Queue_Lock(&frames_stream);
	total = Queue_Get_Total(&frames_stream);
	/* 使用的是选择排序 */
	for(i=0; i<total; ++i) {
		temp = Queue_Get(&frames_stream, i);
		if( !temp ) {
			continue;
		}
		if(temp->current > 0) {
			pos = temp->current-1;
		} else {
			pos = 0;
		}
		
		p = Queue_Get(&temp->pic, pos);
		if( !p ) {
			continue; 
		}
		
		for(j=i+1; j<total; ++j) {
			temp = Queue_Get(&frames_stream, j);
			if( !temp ) {
				continue; 
			}
			if(temp->current > 0) {
				pos = temp->current-1;
			} else {
				pos = 0;
			}
			
			q = Queue_Get(&temp->pic, pos);
			if( !q ) {
				continue; 
			}
			
			if( q->current_time < p->current_time ) {
				Queue_Swap(&frames_stream, j, i);
			}
		}
	}
	Queue_UnLock(&frames_stream);
}

static void 
Frames_Stream_Time_Sub(int time)
/* 功能：将各个动画的当前帧的等待时间与指定时间相减 */
{
	LCUI_Frame *frame;
	LCUI_Frames *frames;
	int i, total, pos;
	
	Queue_Lock(&frames_stream);
	total = Queue_Get_Total(&frames_stream);
	DEBUG_MSG("start\n");
	for(i=0; i<total; ++i) {
		frames = Queue_Get(&frames_stream, i);  
		if( !frames || frames->status == 0 ) {
			continue;
		}
		if(frames->current > 0) {
			pos = frames->current-1;
		} else {
			pos = 0;
		}
		frame = Queue_Get(&frames->pic, pos);
		if( !frame ) {
			continue; 
		}
		frame->current_time -= time; 
		DEBUG_MSG("fames: %p, current: %d, time:%ld, sub:%d\n", 
			frames, pos, frame->current_time, time);
	}
	DEBUG_MSG("end\n");
	Queue_UnLock(&frames_stream);
}

LCUI_Frame *
Frames_Get_Frame(LCUI_Frames *src)
/* 功能：获取当前帧 */
{
	LCUI_Frame *p;
	p = Queue_Get(&src->pic, src->current-1);
	return p;
}

LCUI_Graph *
Frames_Get_Slot(LCUI_Frames *src)
/* 功能：获取当前帧的图像 */
{
	LCUI_Graph *p;
	if( !src ) {
		return NULL;
	}
	p = &src->slot;
	return p;
}

static LCUI_Frames *
Frames_Stream_Update( int *sleep_time )
/* 功能：更新流中的动画至下一帧 */
{ 
	int i, total;
	LCUI_Frame *frame = NULL;
	LCUI_Frames *frames = NULL, *temp = NULL;
	clock_t used_time; 
	
	DEBUG_MSG("start\n");
	total = Queue_Get_Total(&frames_stream); 
	for(i=0; i<total; ++i){
		frames = Queue_Get(&frames_stream, i);
		if(frames->status == 1) {
			break;
		}
	}
	if(i >= total || !frames ) {
		return NULL; 
	}
	/* 
	 * 由于有些动画还未更新第一帧图像，动画槽里的图像也未载入第一帧的图像，因此，
	 * 需要优先处理帧序号为0的动画。
	 * */
	for(i=0; i<total; ++i){
		temp = Queue_Get( &frames_stream, i );
		if( frames->status == 1 && temp->current == 0 ) {
			frames = temp;
			break;
		}
	}
	if( frames && frames->current > 0 ) { 
		frame = Queue_Get( &frames->pic, frames->current-1 );
		if( !frame ) {
			return NULL;
		}
		DEBUG_MSG("current time: %ld\n", frame->current_time);
		if(frame->current_time > 0) {
			*sleep_time = frame->current_time;
			Frames_Stream_Time_Sub( frame->current_time ); 
		}
		
		frame->current_time = frame->sleep_time; 
		++frames->current;
		total = Queue_Get_Total(&frames->pic);
		if(frames->current > total) {
			frames->current = 1;
		}
		frame = Queue_Get(&frames->pic, frames->current-1);
		if( !frame ) {
			return NULL;
		}
	} else {
		frames->current = 1; 
		frame = Queue_Get(&frames->pic, 0);
	}
	
	used_time = clock();/* 开始计时 */
	/* 将该动画当前帧的图像写入至槽中 */
	if(Graph_Valid(&frames->slot)){
		LCUI_Pos pos;
		Graph_Fill_Alpha(&frames->slot, 0);
		if(0 < Queue_Get_Total(&frames->pic)) {
			pos = Frame_Get_Pos(frames, frame);
			Graph_Replace(&frames->slot, frame->pic, pos); 
		}
	}
	used_time = clock()-used_time;
	if(used_time > 0) {
		Frames_Stream_Time_Sub(used_time);
	}
	Frames_Stream_Sort(); /* 重新排序 */
	DEBUG_MSG("current frame: %d\n", frames->current);
	DEBUG_MSG("end\n");
	return frames;
}

static void 
Process_Frames()
/* 功能：处理动画的每一帧的更新 */
{
	int i, total, sleep_time = 10;
	LCUI_Func *func;
	LCUI_Graph *slot;
	LCUI_Frames *frames;
	
	while(!LCUI_Active()) {
		usleep(10000);
	}
	frames = Frames_Stream_Update( &sleep_time ); 
	reset_timer( __timer_id, sleep_time );
	if( frames ) {
		total = Queue_Get_Total( &frames->func_data );
		slot = Frames_Get_Slot( frames ); 
		for(i=0; i<total; ++i){
			func = Queue_Get( &frames->func_data, i );
			func->arg[0] = slot;
			AppTask_Custom_Add( ADD_MODE_REPLACE | AND_ARG_S, func );
		} 
	}
}

int Frames_Play(LCUI_Frames *frames)
/* 功能：播放动画 */
{
	int i, total;
	LCUI_Frames *tmp_ptr;
	if( !frames ) {
		return -1; 
	}
	frames->status = 1;
	if(__timer_id == -1){
		Queue_Init( &frames_stream, sizeof(LCUI_Frames), NULL );
		Queue_Using_Pointer( &frames_stream );
		__timer_id = set_timer( 50, Process_Frames, TRUE );
	}
	/* 检查该动画是否已存在 */
	Queue_Lock( &frames_stream );
	total = Queue_Get_Total( &frames_stream );
	for( i=0; i<total; ++i ) {
		tmp_ptr = Queue_Get( &frames_stream, i );
		if( tmp_ptr == frames ) {
			break;
		}
	}
	Queue_UnLock( &frames_stream );
	/* 添加至动画更新队列中 */ 
	if( i>=total ) {
		return Queue_Add_Pointer(&frames_stream, frames); 
	}
	return 1;
}

int Frames_Pause(LCUI_Frames *frames)
/* 功能：暂停动画 */
{ 
	if( !frames ) {
		return -1;
	}
	frames->status = 0;
	return 0;
}
/*********************** End Frames Process ***************************/

/************************** ActiveBox *********************************/
LCUI_Frames *
ActiveBox_Get_Frames(LCUI_Widget *widget)
{
	LCUI_ActiveBox *actbox;
	actbox = (LCUI_ActiveBox *)Get_Widget_PrivData(widget);
	return actbox->frames;
}

static void 
ActiveBox_Refresh_Frame(LCUI_Graph *frame, void *arg)
/* 功能：刷新动画当前帧的显示 */
{
	LCUI_Widget *widget = (LCUI_Widget*)arg;
	Draw_Widget(widget); 
}

int ActiveBox_Set_Frames_Size(LCUI_Widget *widget, LCUI_Size new_size)
/* 功能：设定动画尺寸 */
{ 
	LCUI_Frames *frames = ActiveBox_Get_Frames(widget);
	return Resize_Frames(frames, new_size); 
}

int ActiveBox_Play(LCUI_Widget *widget)
/* 功能：播放动画 */
{
	LCUI_Frames *frames = ActiveBox_Get_Frames(widget);
	return Frames_Play(frames); 
}

int ActiveBox_Pause(LCUI_Widget *widget)
/* 功能：暂停动画 */
{
	LCUI_Frames *frames = ActiveBox_Get_Frames(widget);
	return Frames_Pause(frames);
}

int ActiveBox_Add_Frame(	LCUI_Widget *widget, LCUI_Graph *pic, 
				LCUI_Pos offset, int sleep_time )
/* 功能：为ActiveBox部件内的动画添加一帧图像 */
{
	LCUI_Frames *frames = ActiveBox_Get_Frames(widget);
	return Add_Frame(frames, pic, offset, sleep_time);
}


static void 
ActiveBox_Init(LCUI_Widget *widget)
/* 功能：初始化ActiveBox部件 */
{
	LCUI_ActiveBox *actbox;
	actbox = (LCUI_ActiveBox *)Widget_Create_PrivData(widget, 
					sizeof(LCUI_ActiveBox)); 
	actbox->frames = Create_Frames(Size(50,50));
	Frames_Add_Func( actbox->frames, ActiveBox_Refresh_Frame, widget );
}

static void 
Exec_Update_ActiveBox(LCUI_Widget *widget)
/* 功能：更新ActiveBox部件内显示的图像 */
{
	LCUI_Rect rect;
	LCUI_Frames *frames = ActiveBox_Get_Frames(widget);
	LCUI_Graph *graph = Frames_Get_Slot(frames);
	LCUI_Pos pos = Align_Get_Pos(Get_Widget_Size(widget), 
				frames->size, ALIGN_MIDDLE_CENTER);

	Graph_Fill_Alpha(&widget->graph, 0);
	Graph_Replace(&widget->graph, graph, pos);  
	rect = Rect(pos.x, pos.y, graph->width, graph->height);
	Add_Widget_Refresh_Area(widget, rect);
}

static void 
Destroy_ActiveBox(LCUI_Widget *widget)
{
	
}

void Register_ActiveBox()
{
	WidgetType_Add("active_box"); 
	WidgetFunc_Add("active_box", ActiveBox_Init, FUNC_TYPE_INIT);
	WidgetFunc_Add("active_box", Exec_Update_ActiveBox, FUNC_TYPE_UPDATE);
	WidgetFunc_Add("active_box", Exec_Update_ActiveBox, FUNC_TYPE_DRAW);
	WidgetFunc_Add("active_box", Destroy_ActiveBox,	 FUNC_TYPE_DESTROY); 
}

/************************** End ActiveBox *****************************/
