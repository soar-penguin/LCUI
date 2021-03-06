/* ***************************************************************************
 * LCUI_Work.c -- LCUI's other work
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
 * LCUI_Work.c -- LCUI 的其它工作
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
#include LC_LCUI_H
#include LC_WORK_H 
#include LC_MISC_H
#include LC_ERROR_H
#include LC_INPUT_H
#include LC_WIDGET_H
#include <unistd.h>

/***************************** Func ***********************************/
void 
NULL_Func()
/* 功能：空函数，不做任何操作 */
{
	return;
}

void 
FuncQueue_Init(LCUI_Queue *queue)
/* 功能：初始化函数指针队列 */
{
	Queue_Init(queue, sizeof(LCUI_Func), NULL);
}

/****************************** Task **********************************/

void 
Send_Task_To_App(LCUI_Func *func_data)
/*
 * 功能：发送任务给程序，使这个程序进行指定任务
 * 说明：LCUI_Func结构体中的成员变量 id，保存的是目标程序的id
 */
{ 
	LCUI_App *app;
	app = Find_App( func_data->id );
	if( !app ) {
		return;
	}
	Queue_Add( &app->task_queue, func_data );
}


BOOL
Have_Task( LCUI_App *app )
/* 功能：检测是否有任务 */
{
	if( !app ) {
		return FALSE; 
	}
	if(Queue_Get_Total(&app->task_queue) > 0) {
		return TRUE; 
	}
	return FALSE;
}

int 
Run_Task( LCUI_App *app )
/* 功能：执行任务 */
{ 
	static LCUI_Task *task;
	task = (LCUI_Task*)Queue_Get( &app->task_queue, 0 );
	//clock_t start = clock();
	//printf("run task %p\n", task->func);
	/* 调用函数指针指向的函数，并传递参数 */
	task->func( task->arg[0], task->arg[1] );
	/* 若需要在调用回调函数后销毁参数 */
	if( task->destroy_arg[0] ) {
		free( task->arg[0] );
	}
	if( task->destroy_arg[1] ) {
		free( task->arg[1] );
	}
	//printf("task %p use time: %ldus\n", task->func, clock()-start);
	return Queue_Delete(&app->task_queue, 0);
}

static void
__destroy_apptask( LCUI_Func *func_data )
/* 销毁程序任务 */
{
	if( func_data->destroy_arg[0] && func_data->arg[0] ) {
		free( func_data->arg[0] );
		func_data->arg[0] = NULL;
	}
	if( func_data->destroy_arg[1] && func_data->arg[1] ) {
		free( func_data->arg[1] );
		func_data->arg[1] = NULL;
	}
}

int 
AppTask_Custom_Add(int mode, LCUI_Func *func_data)
/*
 * 功能：使用自定义方式添加程序任务
 * 用法示例：
 * 在函数的各参数与队列中的函数及各参数不重复时，添加它
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT | AND_ARG_F | AND_ARG_S, func_data);
 * 只要函数和参数1不重复则添加
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT | AND_ARG_F, func_data);
 * 要函数不重复则添加
 * AppTask_Custom_Add(ADD_MODE_NOT_REPEAT, func_data);
 * 添加新的，不管是否有重复的
 * AppTask_Custom_Add(ADD_MODE_ADD_NEW, func_data);
 * 有相同函数则覆盖，没有则新增
 * AppTask_Custom_Add(ADD_MODE_REPLACE, func_data);
 * */
{
	int total, i;
	/* 先获取程序数据结构体指针 */
	LCUI_App *app;
	LCUI_FuncQueue *queue;
	LCUI_Func *temp = NULL;
	
	if( func_data->id == (LCUI_ID)0 ) {
		app = Get_Self_AppPointer();
	} else {
		app = Find_App( func_data->id );
	}
	if( !app ) {
		return -1;
	}
	queue = &app->task_queue;
	total = Queue_Get_Total(queue);
	/* 如果模式是“添加新的”模式 */
	if( mode == ADD_MODE_ADD_NEW ) {
		Queue_Add(queue, func_data); 
		return 0;
	}
	
	//printf("mode: %d\n", mode);
	for (i = 0; i < total; ++i) {
		//printf("1\n");
		temp = Queue_Get(queue, i);
		/* 如果指针无效，或者函数指针已有记录 */
		if( !temp || temp->func != func_data->func ) {
			continue;
		}
		/* 如果要求的是不重复模式 */ 
		if(Check_Option(mode, ADD_MODE_NOT_REPEAT)) {
			/* 如果要求是第1个参数不能重复 */
			if(Check_Option(mode, AND_ARG_F)) {
				//printf("ADD_MODE_NOT_REPEAT, AND_ARG_F\n");
				//printf("old:%p, new:%p\n", queue->queue[i].arg_f, arg_f);
				/* 如果要求是第2个参数也不能重复 */
				if(Check_Option(mode, AND_ARG_S)) {
					/* 如果函数以及参数1和2都一样 */ 
					if(temp->arg[0] == func_data->arg[0] 
					&& temp->arg[1] == func_data->arg[1]) {
						__destroy_apptask( func_data );
						return -1; 
					}
				} else {/* 否则，只是要求函数以及第1个参数不能全部重复 */
					if(temp->arg[0] == func_data->arg[0]) { 
						__destroy_apptask( func_data );
						return -1; 
					}
				}
			}/* 否则，如果只是要求是第2个参数不能重复 */
			else if(Check_Option(mode, AND_ARG_S)) {
				if(temp->arg[1] == func_data->arg[1] ) {
					__destroy_apptask( func_data );
					return -1; 
				}
			} else {/* 否则，只是要求函数不同 */ 
				__destroy_apptask( func_data );
				return -1; 
			}
		}/* 如果要求的是替换模式 */
		else if(Check_Option(mode, ADD_MODE_REPLACE)) {
			//printf("ADD_MODE_REPLACE\n");
			/* 如果要求是第1个参数相同 */
			if( Check_Option(mode, AND_ARG_F) ) {
				/* 如果要求是第2个参数也相同 */
				if( Check_Option(mode, AND_ARG_S) ) {
					if(temp->arg[0] == func_data->arg[0] 
					&& temp->arg[1] == func_data->arg[1]
					) {
						break; 
					}
				} else {/* 否则，只是要求函数以及第1个参数全部相同 */
					if(temp->arg[0] == func_data->arg[0]) {
				//		printf("ARG_F\n");
						break; 
					}
				}
			}/* 否则，如果只是要求第2个参数不能相同 */
			else if(Check_Option(mode, AND_ARG_S)) {
				if(temp->arg[1] == func_data->arg[1]) {
					break; 
				}
			} else { 
				break; 
			}
		}
	}
	
	if(i == total) {
		Queue_Add(queue, func_data); 
	} else {
		Queue_Replace( queue, i, func_data ); 
	}
	return 0;
}
/**************************** Task End ********************************/

/***************************** Event ***********************************/
static void 
Destroy_Event(void *arg)
/* 功能：销毁事件数据 */
{
	LCUI_Event *event = (LCUI_Event*)arg;
	if(event != NULL) {
		Destroy_Queue(&event->func_data); 
	}
}

void 
EventQueue_Init(LCUI_EventQueue * queue)
/* 功能：初始化事件队列 */
{
	/* 
	 * 由于LCUI_Event结构体的成员中有函数队列，销毁事件时需要把该队列销毁，所以需
	 * 要Destroy_Event()作为析构函数 
	 * */
	Queue_Init(queue, sizeof(LCUI_Event), Destroy_Event);
}


BOOL
Get_FuncData(LCUI_Func *p, void (*func) (), void *arg1, void *arg2)
/* 
 * 功能：将函数指针以及两个参数，转换成LCUI_Func类型的指针
 * 说明：此函数会申请内存空间，并返回指向该空间的指针
 *  */
{
	LCUI_App *app;
	app = Get_Self_AppPointer();
	
	if( !app ) {
		printf("Get_FuncData(): "APP_ERROR_UNRECORDED_APP);
		return FALSE;
	}
	
	p->id = app->id;
	p->func = func;
	/* 
	 * 只是保存了指向参数的指针，如果该参数是局部变量，在声明它的函数退出后，该变量
	 * 的空间可能会无法访问。
	 *  */
	p->arg[0] = arg1;	
	p->arg[1] = arg2;
	p->destroy_arg[0] = FALSE;
	p->destroy_arg[1] = FALSE;
	return TRUE;
}

LCUI_Event *
Find_Event(LCUI_EventQueue *queue, int event_id)
/* 功能：根据事件的ID，获取指向该事件的指针 */
{
	LCUI_Event *event; 
	int i, total;  
	total = Queue_Get_Total(queue);
	if (total > 0) {
		for (i = 0; i < total; ++i) {
			event = (LCUI_Event*)Queue_Get(queue, i);
			if(event->id == event_id) {
				return event;
			}
		}
	}
	
	return NULL;
}

int 
EventQueue_Add(LCUI_EventQueue *queue, int event_id, LCUI_Func *func)
/* 功能：记录事件及对应回调函数至队列 */
{
	LCUI_Event *event;
	
	event = Find_Event(queue, event_id);
	if ( !event ) {/* 如果没有，就添加一个新事件类型 */ 
		int pos;
		LCUI_Event new_event;
		new_event.id = event_id;
		Queue_Init(&new_event.func_data, sizeof(LCUI_Func), NULL);
		pos = Queue_Add(queue, &new_event);/* 将新数据追加至队列 */
		event = Queue_Get(queue, pos);	/* 获取指向新增成员的指针 */
	}
	
	event->id = event_id; /* 保存事件ID */
	Queue_Add(&event->func_data, func);
	return 0;
}

int 
LCUI_MouseEvent_Connect (void (*func) (), int event_id)
/* 
 * 功能：将函数与鼠标的相关事件相关联
 * 说明：当鼠标事件触发后，会先将已关联该事件的函数指针及相关事件的指针
 * 添加至程序的任务队列，等待程序在主循环中处理
 **/
{
	LCUI_Func func_data;
	if( !Get_FuncData(&func_data, func, NULL, NULL) ) {
		return -1;
	}
	/* 
	 * 将函数指针及第一个参数加入至鼠标事件处理队列
	 * 中，等到处理鼠标事件时，会将按键状态作为该函
	 * 数的第二个参数并转移至程序的任务队列 
	 **/ 
	EventQueue_Add(&LCUI_Sys.mouse_event, event_id, &func_data); 
	return 0;
}

int 
LCUI_KeyboardEvent_Connect ( void (*func) (LCUI_Key*, void*), void *arg )
/* 与键盘建立连接，当键盘中某个按键被按下/释放后，就会调用这个函数 */
{
	LCUI_Func func_data;
	if( !Get_FuncData(&func_data, func, NULL, arg) ) {
		return -2;
	}
	return Queue_Add( &LCUI_Sys.key_event, &func_data );
}

int 
Handle_Widget_KeyboardEvent( LCUI_Widget *widget, LCUI_Key key ) 
{
	if( !widget ) {
		return -1;
	}
	
	LCUI_Key *key_data;
	LCUI_Event *event;
	LCUI_Func *func;
	int total, i;
	
	event = Find_Event( &widget->event, EVENT_KEYBOARD );
	if( !event ) {
		return -2;
	}
	
	total = Queue_Get_Total( &event->func_data );
	if( total <= 0 ) {
		return 1;
	}
	key_data = (LCUI_Key*)malloc( sizeof(LCUI_Key) );
	memcpy( key_data, &key, sizeof(LCUI_Key) );
	for (i = 0; i < total; ++i) {
		func = Queue_Get( &event->func_data, i );
		/* 为第二个参数分配了内存，需要在调用完回调函数后销毁它 */
		func->arg[1] = key_data;
		func->destroy_arg[1] = TRUE;
		/* 添加至程序的任务队列 */ 
		AppTask_Custom_Add( ADD_MODE_ADD_NEW, func );
	}
	return 0;
}

int 
Handle_Event(LCUI_EventQueue *queue, int event_id)
/* 
 * 功能：处理指定ID的事件
 * 说明：本函数会将事件队列中与指定ID的事件关联的回调函数 添加至程序的任务队列
 * */
{
	LCUI_Event *event;
	LCUI_Func *func;
	int total, i;
	event = Find_Event(queue, event_id);
	if( !event ) {
		return -1;
	}
	total = Queue_Get_Total(&event->func_data);
	for (i = 0; i < total; ++i) {
		func = Queue_Get(&event->func_data, i);
		/* 添加至程序的任务队列 */ 
		AppTask_Custom_Add(ADD_MODE_REPLACE | AND_ARG_F, func);
	}
	return 0;
}


/****************** 处理部件拖动/点击事件的相关代码 ************************/
static LCUI_Widget *click_widget = NULL, *overlay_widget = NULL;
static LCUI_Pos __offset_pos = {0, 0};  /* 点击部件时保存的偏移坐标 */ 
static LCUI_DragEvent drag_event;

int 
Widget_Event_Connect (	LCUI_Widget *widget, 
			WidgetEvent_ID event_id, 
			LCUI_Func *func_data )
/* 将回调函数与部件的指定事件进行关联 */
{
	if( !widget ) {
		return -1;
	}
	EventQueue_Add( &widget->event, event_id, func_data );
	return 0;
}

int 
Widget_Drag_Event_Connect ( LCUI_Widget *widget, 
		void (*func)(LCUI_Widget*, LCUI_DragEvent *)
)
/* 
 * 功能：将回调函数与部件的拖动事件进行连接 
 * 说明：建立连接后，当部件被点击，拖动，释放，都会调用回调函数
 * */
{
	LCUI_DragEvent *p;
	LCUI_Func func_data;
	
	p = &drag_event;
	if( !Get_FuncData(&func_data, func, widget, p) ) {
		return -2;
	}
	return Widget_Event_Connect( widget, EVENT_DRAG, &func_data );
}

int 
Widget_Keyboard_Event_Connect (
			LCUI_Widget *widget,
			void (*func)(LCUI_Widget*, LCUI_Key *)
)
/* 
 * 功能：将回调函数与部件的按键输入事件进行连接 
 * 说明：建立连接后，当部件处于焦点状态，并使用键盘进行输入时，会调用该回调函数
 * */
{
	LCUI_Func func_data;
	
	if( !Get_FuncData(&func_data, func, (void*)widget, NULL) ) {
		return -2;
	}
	return Widget_Event_Connect( widget, EVENT_KEYBOARD, &func_data );
}

int 
Widget_Clicked_Event_Connect (
			LCUI_Widget *widget,
			void (*func)(LCUI_Widget*, void *), 
			void *arg
)
/* 
 * 功能：将回调函数与部件的点击事件进行连接 
 * 说明：建立连接后，当部件被鼠标点击，会调用回调函数
 * */
{
	LCUI_Func func_data;
	
	if( !Get_FuncData(&func_data, func, (void*)widget, arg) ) {
		return -2;
	}
	return Widget_Event_Connect( widget, EVENT_CLICKED, &func_data );
}

static LCUI_Widget *
Get_ResponseStatusChange_Widget(LCUI_Widget *widget)
/* 
 * 功能：查找能响应状态改变的部件 
 * 说明：此函数用于检查部件以及它的上级所有父部件，第一个有响应状态改变的部件的指针将会
 * 作为本函数的返回值。
 * */
{
	if( !widget ) {
		return NULL;
	}
	if( widget->status_response ) {
		return widget;/* 如果部件响应状态改变，那就返回该部件的指针 */
	}
	if( !widget->parent ) {
		return NULL; /* 如果父部件为空，那就没找到，返回NULL */
	} else {/* 否则，在它的父级部件中找 */
		return Get_ResponseStatusChange_Widget(widget->parent); 
	}
}

static BOOL
Widget_Have_Event(LCUI_Widget *widget, int event_id)
/* 检测部件是否关联了指定事件 */
{ 
	LCUI_Event *event;
	
	if( !widget ) {
		return FALSE;
	}
	event = Find_Event( &widget->event, event_id );
	if( !event ) {
		return FALSE;
	}
	return TRUE;
}

static LCUI_Widget *
Get_ResponseEvent_Widget(LCUI_Widget *widget, int event_id)
/* 
 * 功能：查找能响应事件的部件 
 * 说明：此函数用于检查部件以及它的上级所有父部件，第一个有响应指定事件的部件，它的指针
 * 将会作为本函数的返回值
 * */
{
	if( !widget ) {
		return NULL;
	}
	if( Widget_Have_Event(widget, event_id) ) { 
		return widget; 
	}
	if( !widget->parent ) { 
		return NULL;
	} else {
		return Get_ResponseEvent_Widget(widget->parent, event_id); 
	}
}

static void 
_Start_DragEvent( LCUI_Widget *widget, LCUI_MouseEvent *event )
{
	drag_event.cursor_pos = event->global_pos;
	/* 用全局坐标减去部件的全局坐标，得到偏移坐标 */ 
	__offset_pos = Pos_Sub( event->global_pos, Get_Widget_Global_Pos( widget ) );
	/* 得出部件的新全局坐标 */
	drag_event.new_pos = Pos_Sub( event->global_pos, __offset_pos );
	drag_event.first_click = 1;
	drag_event.end_click = 0;
	/* 处理部件的拖动事件 */
	Handle_Event( &widget->event, EVENT_DRAG );
}

static void 
_Doing_DragEvent( LCUI_Widget *widget, LCUI_MouseEvent *event )
{
	drag_event.cursor_pos = event->global_pos;
	drag_event.new_pos = Pos_Sub( event->global_pos, __offset_pos );
	drag_event.first_click = 0;
	drag_event.end_click = 0;
	Handle_Event( &widget->event, EVENT_DRAG );
}

static void 
_End_DragEvent( LCUI_Widget *widget, LCUI_MouseEvent *event )
{
	drag_event.cursor_pos = event->global_pos;
	drag_event.new_pos = Pos_Sub( event->global_pos, __offset_pos );
	drag_event.first_click = 0;
	drag_event.end_click = 1;
	Handle_Event( &widget->event, EVENT_DRAG );
}

static void 
Widget_Clicked(LCUI_MouseEvent *event)
/*
 * 功能：用于处理click事件，并保存被点击后的部件的指针
 * 说明：在鼠标左键被按下/释放时，都会调用这个函数
 **/
{
	LCUI_Widget *widget;
	if( !event ) {
		return;
	}
	
	widget = event->widget;
	switch( Mouse_LeftButton(event) ) {
	    case PRESSED:
		DEBUG_MSG("mouse left button pressed\n");
		click_widget = widget;
		/* 焦点转移给该部件 */
		Set_Focus( widget );
		if( Widget_Have_Event( widget, EVENT_DRAG ) ) {
			/* 开始处理部件的拖动 */
			DEBUG_MSG("widget have EVENT_DRAG\n");
			_Start_DragEvent( widget, event );
		}
		widget = Get_ResponseStatusChange_Widget( widget ); 
		if( !widget ) {
			DEBUG_MSG("widget not response status change\n");
			break;
		}
		/* 如果当前鼠标指针覆盖到的部件已被启用 */  
		if( event->widget->enabled && widget->enabled ) {
			Set_Widget_Status (widget, WIDGET_STATUS_CLICKING); 
		} else {
			Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
		}
		break;
		
	    case FREE: 
		DEBUG_MSG("mouse left button free\n");
		if( !click_widget ) {
			/* 如果是点击屏幕空白处，则复位焦点 */
			Reset_Focus( NULL );
			break;
		}
		if( Widget_Have_Event( click_widget, EVENT_DRAG ) ) {
			DEBUG_MSG("end drag\n");
			_End_DragEvent( click_widget, event );
		}
		if(click_widget == widget) {
			click_widget = NULL;
			/* 如果点击时和点击后都在同一个按钮部件内进行的,
			 * 触发CLICKED事件，将部件中关联该事件的回调函数发送至
			 * 任务队列，使之在主循环中执行 
			 * */
			
			widget = Get_ResponseEvent_Widget( event->widget, EVENT_CLICKED );
			if( widget && widget->enabled ) {
				Handle_Event(&widget->event, EVENT_CLICKED);
			}
			widget = Get_ResponseStatusChange_Widget(event->widget);
			if( !widget ) {
				break;
			}
			if(widget->enabled) {
				Set_Widget_Status (widget, WIDGET_STATUS_CLICKED);
				Set_Widget_Status (widget, WIDGET_STATUS_OVERLAY);
				break;
			}
			Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
			break;
		}
		/* 否则，将恢复之前点击的鼠标的状态 */ 
		widget = Get_ResponseStatusChange_Widget(click_widget);
		click_widget = NULL;
		if( !widget ) {
			break;
		}
		if(widget->enabled) {
			Set_Widget_Status (widget, WIDGET_STATUS_NORMAL);
		} else {
			Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
		}
	}
}

static void 
Tracking_Mouse_Move (LCUI_MouseEvent *event)
/* 
 * 功能：跟踪鼠标移动，处理触发的基本事件
 * 说明：这只是根据鼠标事件来处理部件状态的切换
 * */
{
	LCUI_Widget *widget;
	
	/* 获取当前鼠标游标覆盖到的部件的指针 */
	widget = Get_Cursor_Overlay_Widget();
	if( !widget ) {
		goto skip_widget_check;
	}
	/* 获取能响应状态改变的部件的指针 */
	widget = Get_ResponseStatusChange_Widget(widget); 
	if( !widget || overlay_widget == widget ) {
		goto skip_widget_check;
	}
	if ( widget->enabled ) {
		DEBUG_MSG("widget not enabled");
		if( !click_widget  ) {
			DEBUG_MSG("leftbutton is free, widget overlay\n\n");
			Set_Widget_Status (widget, WIDGET_STATUS_OVERLAY);
		}
	} else {
		Set_Widget_Status (widget, WIDGET_STATUS_DISABLE);
	}
	
	/* 如果之前有覆盖到的部件 */
	if (overlay_widget && !click_widget ) {
		if ( overlay_widget->enabled ) {
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_NORMAL);
		} else {
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_DISABLE);
		}
	}
	/* 保存当前覆盖到的按钮部件指针 */
	overlay_widget = widget;
	return;

skip_widget_check:;

	/* 如果鼠标指针在之前有覆盖到的部件 */
	if( overlay_widget != widget && !click_widget ) {
		if ( overlay_widget->enabled ) {
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_NORMAL); 
		} else {
			Set_Widget_Status (overlay_widget, WIDGET_STATUS_DISABLE);
		}
	} 
	overlay_widget = widget;
	/* 如果之前点击过部件，并且现在鼠标左键还处于按下状态，那就处理部件拖动 */ 
	if( click_widget && Mouse_LeftButton (event) == PRESSED 
	 && Widget_Have_Event( click_widget, EVENT_DRAG ) ) {
		_Doing_DragEvent( click_widget, event );
	}
}

static void 
WidgetFocusProc( LCUI_Key *key_data, void *arg );

void 
Widget_Event_Init()
/* 功能：初始化部件事件处理 */
{
	LCUI_MouseEvent_Connect( Tracking_Mouse_Move, MOUSE_EVENT_MOVE );
	LCUI_MouseEvent_Connect( Widget_Clicked, MOUSE_EVENT_CLICK );
	LCUI_KeyboardEvent_Connect( WidgetFocusProc, NULL );
}

/*************************** Event End *********************************/


/*--------------------------- Focus Proc ------------------------------*/

BOOL 
Set_Focus( LCUI_Widget *widget )
/* 
 * 功能：为部件设置焦点
 * 说明：上个获得焦点的部件会得到EVENT_FOCUS_OUT事件，而当前获得焦点的部件会得到
 * EVENT_FOCUS_IN事件。
 * */
{
	if( widget ) {
		/* 先处理上级部件的焦点 */
		if( widget->parent ) {
			Set_Focus( widget->parent );
		}
		if( !widget->focus ) {
			return FALSE;
		}
	} else {
		return FALSE;
	}
	
	LCUI_Widget **focus_widget;
	
	if( widget->parent ) {
		focus_widget = &widget->parent->focus_widget;
	} else {
		focus_widget = &LCUI_Sys.focus_widget; 
	}
	if( *focus_widget ) {
		/* 如果上次和这次的部件不一样 */
		if( *focus_widget != widget ) {
			Handle_Event( &(*focus_widget)->event, EVENT_FOCUS_OUT );
		}
	}
	Handle_Event( &widget->event, EVENT_FOCUS_IN );
	/* 保存新焦点位置 */
	*focus_widget = widget;
	return TRUE;
}

BOOL 
Cancel_Focus( LCUI_Widget *widget )
/* 
 * 功能：取消指定部件的焦点
 * 说明：该部件会得到EVENT_FOCUS_OUT事件，并且，会将焦点转移至其它部件
 * */
{
	if( !widget || !widget->focus ) {
		return FALSE;
	}
	
	int i, total, focus_pos;
	LCUI_Widget *other_widget, **focus_widget;
	LCUI_Queue *queue_ptr;
	
	if( widget->parent ) {
		focus_widget = &widget->parent->focus_widget;
		queue_ptr = &widget->parent->child;
	} else {
		focus_widget = &LCUI_Sys.focus_widget;
		queue_ptr = &LCUI_Sys.widget_list;
	}
	/* 寻找可获得焦点的其它部件 */
	total = Queue_Get_Total( queue_ptr );
	focus_pos = WidgetQueue_Get_Pos( queue_ptr, *focus_widget );
	for( i=0; i<focus_pos; ++i ) {
		other_widget = Queue_Get( queue_ptr, i);
		if( other_widget && other_widget->visible
		 && other_widget->focus ) {
			Handle_Event( &widget->event, EVENT_FOCUS_IN );
			*focus_widget = other_widget;
			break;
		}
	}
	if( i < focus_pos ) {
		return TRUE;
	}
	/* 排在该部件前面的符合条件的部件没找到，就找排在该部件后面的 */
	for( i=focus_pos+1; i<total; ++i ) {
		other_widget = Queue_Get( queue_ptr, i);
		if( other_widget && other_widget->visible
		 && other_widget->focus ) {
			Handle_Event( &widget->event, EVENT_FOCUS_IN );
			*focus_widget = other_widget;
			break;
		}
	}
	/* 没找到就复位焦点 */
	if( i >= total ) {
		*focus_widget = NULL;
	}
	return TRUE;
}

BOOL
Reset_Focus( LCUI_Widget* widget )
/* 复位指定部件内的子部件的焦点 */
{	
	LCUI_Widget** focus_widget;
	
	if( widget ) {
		focus_widget = &widget->focus_widget; 
	} else {
		focus_widget = &LCUI_Sys.focus_widget; 
	}
	if( *focus_widget ) {
		Handle_Event( &(*focus_widget)->event, EVENT_FOCUS_OUT );
	}
	
	*focus_widget = NULL;
	return TRUE;
}

int Next_FocusWidget()
{
	return 0;
}

int Prev_FocusWidget()
{
	return 0;
}

int Return_FocusToParent()
{
	return 0;
}

BOOL 
Widget_FocusIn_Event_Connect(	LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, void*), 
				void *arg )
/* 将回调函数与FOCUS_IN事件连接，当部件得到焦点时，会调用该回调函数 */
{
	if( !widget ) {
		return FALSE;
	}
	
	LCUI_Func func_data;
	
	if( !Get_FuncData(&func_data, func, (void*)widget, arg) ) {
		return FALSE;
	}
	EventQueue_Add(&widget->event, EVENT_FOCUS_IN, &func_data);
	return TRUE;
}

BOOL 
Widget_FocusOut_Event_Connect(	LCUI_Widget *widget, 
				void (*func)(LCUI_Widget*, void*), 
				void *arg )
/* 将回调函数与FOCUS_OUT事件连接，当部件失去焦点时，会调用该回调函数 */
{
	if( !widget ) {
		return FALSE;
	}
	
	LCUI_Func func_data;
	
	if( !Get_FuncData(&func_data, func, (void*)widget, arg) ) {
		return FALSE;
	}
	EventQueue_Add(&widget->event, EVENT_FOCUS_OUT, &func_data);
	return TRUE;
}

static void 
WidgetFocusProc( LCUI_Key *key_data, void *arg )
{
	LCUI_Widget *widget, *tmp = NULL, *focus_widget;
	
	widget = NULL;
	//printf("key, code: %d, status:%d\n", 
	//	key_data->code, key_data->status);
	while( 1 ) {
		focus_widget = Get_FocusWidget( widget );
		if( !focus_widget ) {
			if( tmp ) {
				Handle_Widget_KeyboardEvent( tmp, *key_data );
			}
			break;
		}
		widget = focus_widget;
		/* 保存已关联按键事件的部件指针 */
		if( Find_Event( &widget->event, EVENT_KEYBOARD ) ) {
			tmp = widget;
		}
	}
}
/*------------------------- End Focus Proc ----------------------------*/
