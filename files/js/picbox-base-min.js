﻿eval(function(p,a,c,k,e,r){e=function(c){return(c<62?'':e(parseInt(c/62)))+((c=c%62)>35?String.fromCharCode(c+29):c.toString(36))};if('0'.replace(0,e)==0){while(c--)r[e(c)]=k[c];k=[function(e){return r[e]||e}];e=function(){return'([369ehpquyzBD-SU-Y]|[1-3]\\w)'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}('6 openShare(2Q,2R,2d,2S,2T){3 E="2U://2V.2W.2X/interface/fromsoft/?action=share&issoft=0";E+="&plat="+2Q;E+="&songname="+2e(2R);E+="&2d="+2d;E+="&duration="+2S;E+="&s="+2T;Q.2f(E)}1t.1g.2Y=6(){3 1P=0;J(3 i=0;i<h.y;i++){9(h.2g(i)>2Z){1P+=2}G{1P+=1}}e 1P};1t.1g.intercept=6(y,1Q){3 I=h;I=I.30();9(I.2Y()<y)e I;3 1R=0;3 2h=0;9(1Q.y>0){y=y-1Q.y}J(3 i=0;i<I.y;i++){9(h.2g(i)>2Z){1R+=2}G{1R+=1}9(1R>y){2i}2h++}e I.2j(0,2h)+1Q};1t.1g.30=6(){e h.1o(/^\\s*|\\s*$/g,"")};6 sdnClick(S){1u{setTimeout(6(){(W Image()).31="2U://sdn.2W.2X/link.aspx?id="+S+"&E=&t="+1S.random()},0)}1v(ex){}};Request={QueryString:6(1T){3 19=32.1U.33(W 2l("[\\?\\&]"+1T+"=([^\\&]*)(\\&?)","i"));e 19?19[1]:19},SharpString:6(1T){3 19=32.href.33(W 2l("[\\#]"+1T+"=([^\\&]*)(#?)","i"));e 19?19[1]:19}};3 Kg=Kg||{UA:{Ie:!!q.1w,Ie6:!!q.1w&&!Q.2o,Ie7:!!q.1w&&/34 7.0/gi.1a(Q.1y.35),Ie8:!!q.1w&&/34 8.0/gi.1a(Q.1y.35),FF:/firefox/gi.1a(Q.1y.2p),Opera:/opera/gi.1a(Q.1y.2p),36:/36/gi.1a(Q.1y.2p)},$:6(){3 1z=[];J(3 i=0,l=1V.y;i<l;i++){3 p=1V[i];9(X p=="1W")p=q.getElementById(p);9(l==1)e p;1z.1b(p)};e 1z},$T:6(1c,p){3 1z=(h.$(p)||q).37(1c||"*");e h.$A(1z)},$C:6(z,p,1c){3 1A=[],i=0;9(!!q.38){3 D=h.$(p||q).38(z);D=h.$A(D);9(1c&&1c!=="*"){J(3 l=D.y;i<l;i++){(D[i].1c.39()===1c.39())&&1A.1b(D[i])}}G{1A=D}}G{J(3 D=h.$T(1c,p),l=D.y;i<l;i++){W 2l("\\\\b"+z+"\\\\b","g").1a(D[i].className)&&1A.1b(D[i])}};e 1A},$A:6(L){3 D=[];J(3 i=0,l=L.y;i<l;i++){D.1b(L[i])};e D},extend:6(1B,1X,3a){J(3 1q in 1X){9(3a)1B[1q]=1X[1q];G 9(!1B[1q])1B[1q]=1X[1q]};e 1B},getStyle:6(p,z){p=h.$(p);9(z==="float"){z=Kg.UA.Ie?"styleFloat":"cssFloat"};z=z.1o(/-(\\w)/,6(a,b){e b.3b()});e Kg.UA.Ie?p.currentStyle[z]:Q.getComputedStyle(p,M)[z]},3c:6(){9(q.3d=="BackCompat"){3 2q=q.1d.3e;3 2r=q.1d.3f;3 2s=q.1d.3g;3 2t=q.1d.3h;3 2u=q.1d.2v;3 2w=q.1d.2x}G 9(q.3d=="CSS1Compat"){3 2q=q.1r.3e;3 2r=q.1r.3f;3 2s=q.1r.3g;3 2t=q.1r.3h;3 2u=q.1d.2v||q.1r.2v;3 2w=q.1d.2x||q.1r.2x};e{cH:2q,cW:2r,sH:2s,sW:2t,sT:2u,sL:2w}},getXY:6(p){p=h.$(p);3 1D=h.3c();3 1E=p.getBoundingClientRect();e{2A:1D.sL+1E.2A,2B:1D.sL+1E.2B,2C:1D.sT+1E.2C,2D:1D.sT+1E.2D}},isFather:6(1h,Y,3i){1h=h.$(1h);Y=h.$(Y);9(3i&&(1h==Y))e 1i;9(1h.3j)e 1h.3j(Y)==20;while(Y&&Y.3k){Y=Y.3k;9(Y==1h)e 1i};e 1Y},addEvent:6(K,15,17){K=h.$(K);9(K.3l){K.3l(15,17,1i)}G 9(K.3m){K.3m("on"+15,17)}G{K["on"+15]=17}},removeEvent:6(K,15,17){K=h.$(K);9(K.3n){K.3n(15,17,1Y)}G 9(K.3o){K.3o("on"+15,17)}G{K["on"+15]=M}},bind:6(17,3p){3 F=3q.1g.3r.3s(1V,2);e 6(){17.3t(3p,F.concat(3q.1g.3r.3s(1V)))}},stopEvent:6(1j){1j=Q.1j||1j;9(1j.3u){1j.3u()}G{1j.cancelBubble=1i};e h},inArray:6(D,21){J(3 i=0,l=D.y;i<l;i++){9(D[i]===21)e 1i};e 1Y},2E:6(D,21){J(3 i=0,l=D.y;i<l;i++){9(D[i]===21)e i};e-1},2F:6(p,S){p=h.$(p);q.1w?p.R.1e="Alpha(Opacity="+S+")":p.R.opacity=S/2G;e p},fadein:6(p,N,1f,B){N=N||1;1f=1f||1;p=h.$(p);3 S=0;3 22=h;3 11=2H(6(){22.2F(p,(S+=1f));9(S>=2G){23(11);B&&B(p)}},N);e 11},fadeout:6(p,N,1f,B){N=N||1;1f=1f||1;p=h.$(p);3 S=2G;3 22=h;3 11=2H(6(){22.2F(p,(S-=1f));9(S<=0){23(11);B&&B(p)}},N);e 11},slide:6(p,R,12,13,N,B,1F){p=h.$(p);N=N||0.1;3 25="";3 1G=p;9(R==="height"||R==="width"||R==="2C"||R==="2D"||R==="2A"||R==="2B"){p=p.R;25="px"};3 11=2H(6(){9(12>13){12-=1S.3v((12-13)*N);p[R]=12+25;1F&&1F(1G);9(12<=13){23(11);B&&B(1G)}}G{12+=1S.3v((13-12)*N);p[R]=12+25;1F&&1F(1G);9(12>=13){23(11);B&&B(1G)}}},1);e 11},3w:6(){6 f(n){e n<10?\'0\'+n:n};3x.1g.2I=6(){e h.getUTCFullYear()+\'-\'+f(h.getUTCMonth()+1)+\'-\'+f(h.getUTCDate())+\'T\'+f(h.getUTCHours())+\':\'+f(h.getUTCMinutes())+\':\'+f(h.getUTCSeconds())+\'Z\'};3 m={\'\\b\':\'\\\\b\',\'\\t\':\'\\\\t\',\'\\n\':\'\\\\n\',\'\\f\':\'\\\\f\',\'\\r\':\'\\\\r\',\'"\':\'\\\\"\',\'\\\\\':\'\\\\\\\\\'};6 18(u,1k){3 a,i,k,l,r=/["\\\\\\x00-\\x1f\\x7f-\\x9f]/g,v;3y(X u){1l\'1W\':e r.1a(u)?\'"\'+u.1o(r,6(a){3 c=m[a];9(c){e c};c=a.2g();e\'\\\\u00\'+1S.floor(c/16).2J(16)+(c%16).2J(16)})+\'"\':\'"\'+u+\'"\';1l\'3z\':e isFinite(u)?1t(u):\'M\';1l\'boolean\':1l\'M\':e 1t(u);1l\'26\':9(!u){e\'M\'};9(X u.2I===\'6\'){e 18(u.2I())};a=[];9(X u.y===\'3z\'&&!(u.propertyIsEnumerable(\'y\'))){l=u.y;J(i=0;i<l;i+=1){a.1b(18(u[i],1k)||\'M\')};e\'[\'+a.2K(\',\')+\']\'};9(1k){l=1k.y;J(i=0;i<l;i+=1){k=1k[i];9(X k===\'1W\'){v=18(u[k],1k);9(v){a.1b(18(k)+\':\'+v)}}}}G{J(k in u){9(X k===\'1W\'){v=18(u[k],1k);9(v){a.1b(18(k)+\':\'+v)}}}};e\'{\'+a.2K(\',\')+\'}\'}};e{18:18,27:6(1H,1e){3 j;6 2L(k,v){3 i,n;9(v&&X v===\'26\'){J(i in v){9(Object.1g.hasOwnProperty.3t(v,[i])){n=2L(i,v[i]);9(n!==3A){v[i]=n}G{delete v[i]}}}};e 1e(k,v)};9(/^[\\],:{}\\s]*$/.1a(1H.1o(/\\\\["\\\\\\/bfnrtu]/g,\'@\').1o(/"[^"\\\\\\n\\r]*"|1i|1Y|M|-?\\d+(?:\\.\\d*)?(?:[eE][+\\-]?\\d+)?/g,\']\').1o(/(?:^|:|,)(?:\\s*\\[)+/g,\'\'))){j=eval(\'(\'+1H+\')\');e X 1e===\'6\'?2L(\'\',j):j};throw W SyntaxError(\'parseJSON\')}}}(),Cookie:{1s:6(z,u,U,P){3 1I="";9(U){3 dt=W 3x();dt.setTime(dt.getTime()+(U*1000));1I=";1I="+dt.toGMTString()};9(P!=3A){q.1m=z+"="+u+1I+";P="+P+";"}G{q.1m=z+"="+u+1I+";P=\\;"}},setDay:6(z,u,U,P){h.1s(z,u,(U*24*60*60),P)},setHour:6(z,u,U,P){h.1s(z,u,(U*60*60),P)},setMin:6(z,u,U,P){h.1s(z,u,(U*60),P)},setSec:6(z,u,U,P){h.1s(z,u,(U),P)},read:6(z,1K){3 29="";3 1U=z+"=";9(q.1m.y>0){1L=q.1m.2E(1U);9(1L!=-1){1L+=1U.y;13=q.1m.2E(";",1L);9(13==-1){13=q.1m.y};29=q.1m.substring(1L,13)}};9(1K){e W Kg.3B().27(29)[1K]};e 29},remove:6(z){h.1s(z,"",-1)}},3B:6(){3 D=[];3 o={};h.27=6(I){3 a=I.3C("&");J(3 i=0,l=a.y;i<l;i++){3 k=a[i].3C("=");o[k[0]]=k[1]};e o};h.2J=6(1e){1e=1e||"&";e D.2K(1e)};h.add=6(1K,3D){3 3E=1K+"="+3D;D.1b(3E);e h}},1M:6(1n,E,O,L,B,V,1N){1u{3F.3G(40,\'{"2a":"1", "N":"80", "3H":"5"}\')}1v(ex){}3 F=L;O=O==M?1i:O;9(L){F=2e(2e(F));9(X L==="26"){3 I="";J(3 i in L){I+=i+"="+L[i]+"&"};F=I.2j(0,I.y-1)}};1n=1n?1n.3b():"POST";1N=1N?1N:"1H";3 H=M;9(Q.2o&&!(Q.2b)){H=W 2o()}G 9(Q.2b){1u{H=W 2b("Microsoft.3I")}1v(otherMSIE){1u{H=W 2b("Msxml2.3I")}1v(NoSupport){H=M}}};H.2M=6(){9(H.2c==4){1u{3F.3G(40,\'{"2a":"2", "N":"0", "3H":"0"}\')}1v(ex){}9(H.2a==200||H.2a==0){3 1O=M;3y(1N){1l"xml":1O=H.responseXML;2i;1l"2N":1O=Kg.3w.27(H.3J);2i;default:1O=H.3J};B&&B(1O);H=M}G{V&&V()}}};9(1n=="GET"){H.2f(1n,E+"?"+(F||\'\'),O);H.3K(M)}G{H.2f(1n,E,O);H.setRequestHeader("Content-Type","application/x-2V-form-urlencoded");H.3K(F)}},2O:6(E,F,B,V,O){h.1M("2O",E,O,F,B,V)},2P:6(E,F,B,V,O){h.1M("2P",E,O,F,B,V)},getJSON:6(E,F,B,V,O){h.1M("2O",E,O,F,B,V,"2N")},postJSON:6(E,F,B,V,O){h.1M("2P",E,O,F,B,V,"2N")},loadScript:6(E,L,B){3 F=L||"";9(L&&(X L==="26")){3 I="";J(3 i in L){I+=i+"="+L[i]+"&"};F=I.2j(0,I.y-1)};3 14=q.createElement("14");14.15=\'1H/javascript\';14.31=E+"?"+F;14.3L=14.2M=6(){9(!h.2c||(h.2c=="complete"||h.2c=="loaded")){B&&B();14.2M=14.3L=M;14=M}};q.37("head")[0].appendChild(14)}};',[],234,'|||var|||function|||if|||||return|||this||||||||el|document||||value||||length|name||callback||arr|url|params|else|XMLHttp|str|for|obj|args|null|speed|async|path|window|style|num||exp|error|new|typeof|child|||timer|start|end|script|type||func|stringify|svalue|test|push|tagName|body|filter|step|prototype|father|true|event|whitelist|case|cookie|method|replace||property|documentElement|write|String|try|catch|all||navigator|els|cEls|target||bodySize|elRect|extra|dom|text|expires||key|offset|Ajax|docType|param|bytes|appendStr|countLen|Math|item|search|arguments|string|souce|false|||compare|_this|clearInterval||prefix|object|parse||cookieValue|status|ActiveXObject|readyState|hash|encodeURI|open|charCodeAt|charCount|break|substr||RegExp|||XMLHttpRequest|userAgent|clientH|clientW|scrollH|scrollW|scrollT|scrollTop|scrollL|scrollLeft|||left|right|top|bottom|indexOf|setOpacity|100|setInterval|toJSON|toString|join|walk|onreadystatechange|json|get|post|chl|file|timeLen|source|http|www|kugou|com|getBytes|256|trim|src|location|match|msie|appVersion|Chrom|getElementsByTagName|getElementsByClassName|toLowerCase|rewrite|toUpperCase|getBodySize|compatMode|clientHeight|clientWidth|scrollHeight|scrollWidth|bol|compareDocumentPosition|parentNode|addEventListener|attachEvent|removeEventListener|detachEvent|environment|Array|slice|call|apply|stopPropagation|ceil|JSON|Date|switch|number|undefined|Param|split|val|prm|external|SuperCall|autotime|XMLHTTP|responseText|send|onload'.split('|'),0,{}))
try{document.execCommand('BackgroundImageCache', false, true);}catch(e){};
eval(function(p,a,c,k,e,r){e=function(c){return(c<62?'':e(parseInt(c/62)))+((c=c%62)>35?String.fromCharCode(c+29):c.toString(36))};if('0'.replace(0,e)==0){while(c--)r[e(c)]=k[c];k=[function(e){return r[e]||e}];e=function(){return'[3-9b-hj-zA-U]'};c=1};while(c--)if(k[c])p=p.replace(new RegExp('\\b'+e(c)+'\\b','g'),k[c]);return p}('7 Page=h(5,r,g,d,j){3.5=parseInt(5);3.r=r;3.E="首页";3.F="上一页";3.G="下一页";3.H="尾页";3.g=g;3.s=I;3.t=I;3.d=d;3.b=J.ceil(3.r/3.d);3.j=j;3.C=true;3.K=m;3.GetText=h(){6(3.b==0||3.5>3.b){9""}7 k="<c e=\\"mypage\\">";3.L();k+=3.M();k+=3.N();k+=3.O();k=k+"</c>";9 k};3.M=h(){7 4="";7 w="";6(3.5>1){7 8=3.5-1==1?"1":(3.5-1);6(!3.j){6(3.C){4="<a e=\\"page_first\\" x=\\"首页\\" f=\\"y z\\" l=\\"n:o(0)\\" p=\\"u(1,"+3.d+");9 m;\\" >"+3.E+"</a>"}4+="<c f=\\"PrePageSpan\\" P=\\"Q:A;R:A;\\" ><a e=\\"page_pre_"+8+"\\" l=\\"n:o(0)\\" x=\\"上一页\\" f=\\"y z\\" p=\\"u("+8+","+3.d+");9 m;\\" >"+3.F+"</a></c>"}}9 4};3.O=h(){7 4="";7 w="";6(3.5>=1&&3.5!=3.r){7 8=3.5+1;7 B=3.b;6(8>B){4="";6(3.K){4+="<c f=\'page_search\'>Pages："+3.5+"/"+3.b+"&nbsp;<S type=\'textbox\' e=\'inputSearch\' f=\'S\'><a l=\'n:o(0)\' p=\'pageGo(3)\'>Go</a></c>"}}q{6(!3.j){4="<c f=\\"NextPageSpan\\" P=\\"Q:A;R:A;\\" ><a e=\\"page_next_"+8+"\\" l=\\"n:o(0)\\"  x=\\"下一页\\" f=\\"y z\\" p=\\"u("+8+","+3.d+");9 m;\\"  >"+3.G+"</a></c>";6(3.C){4+="<a e=\\"page_last_"+B+"\\" x=\\"尾页\\" l=\\"n:o(0)\\" f=\\"y z\\" p=\\"u("+B+","+3.d+");9 m;\\" >"+3.H+"</a>"}}}}9 4};3.N=h(){7 4="";6(3.b<=3.g){4=4+3.v(1,3.b)}q{6(3.5<=3.t){4=4+3.v(1,3.g)}q 6(5<=(3.b-3.t)){4=4+3.v((3.5-3.s),(3.5+3.t))}q{4=4+3.v((3.5-(3.g-((3.b-3.5)+1))),3.b)}}9 4};3.v=h(T,U){7 4="";7 w="";for(i=T;i<=U;i++){6(i==3.5){4=4+"<c e=\\"D"+i+"\\" f=\\"current\\">"+i+"</c>"}q{7 8=(i==1?1:i);6(3.j){4=4+"<a e=\\"D"+8+"\\" l = \\""+3.j+"_"+8+w+".htm\\" >"+i+"</a>"}q{4=4+"<a e=\\"D"+8+"\\" l=\\"n:o(0)\\" p=\\"u("+8+","+3.d+");9 m;\\" >"+i+"</a>"}}}9 4};3.L=h(){3.s=J.floor(3.g/2);3.t=3.s;6(3.g%2==0)3.s--}}',[],57,'|||this|str|CurrentPage|if|var|temp|return||PageCount|span|PageSize|id|class|DisplayPages|function||Url|pageStr|href|false|javascript|void|onclick|else|RecordCount|_padLeft|_padRight|GotoNext|RenderLinkRange|addition|title|direct|btnPage|0px|temp1|IsShowFirstAndLast|page_|First|Prev|Next|Last|null|Math|IsShowPageSearch|Padding|RenderBeginTag|RenderPagingContents|RenderEndTag|style|border|padding|input|start|end'.split('|'),0,{}))