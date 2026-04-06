#include <WiFi.h>
#include <WebServer.h>
#include "USB.h"
#include "USBHIDMouse.h"
#include "USBHIDKeyboard.h"
#include "USBHIDConsumerControl.h"

USBHIDMouse Mouse;
USBHIDKeyboard Keyboard;
USBHIDConsumerControl Consumer;
WebServer server(80);

const char* ssid = "ESP32";
const char* password = "12345678";

const char webpage[] PROGMEM = 
"<!DOCTYPE html>\n"
"<html>\n"
"<head>\n"
"<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n"
"<style>\n"
"body{margin:0;background:#111;color:#ddd;font-family:Arial,sans-serif;text-align:center}\n"
"h3{margin:8px;color:#0f0}\n"
"#pad{width:95vw;height:170px;background:#1a1a1a;margin:8px auto;border-radius:12px;border:2px solid #333;}\n"
"textarea{width:92vw;height:140px;background:#222;color:#fff;border:2px solid #555;border-radius:8px;padding:10px;font-size:15px;margin:8px auto;display:block;resize:vertical}\n"
"#sendBtn,#clearBtn{padding:12px 24px;font-size:18px;color:#fff;border:none;border-radius:8px;margin:8px 4px;cursor:pointer}\n"
"#sendBtn{background:#0a0}#sendBtn:active{background:#080}\n"
"#clearBtn{background:#555}#clearBtn:active{background:#333}\n"
".keyboard{display:grid;gap:3px;max-width:98vw;margin:12px auto;padding:8px;background:#1f1f1f;border-radius:12px;box-shadow:0 0 12px rgba(0,0,0,.7)}\n"
".row{display:flex;gap:3px;justify-content:center}\n"
".key{background:#333;color:#fff;border:1px solid #555;border-radius:5px;display:flex;align-items:center;justify-content:center;font-size:12px;cursor:pointer;user-select:none;padding:1px;box-shadow:0 2px 3px rgba(0,0,0,.6)}\n"
".key:active{background:#555;transform:translateY(1px)}\n"
".held{background:#0a0 !important;color:#fff !important;box-shadow:0 0 8px #0f0 !important}\n"
".k1{flex:1;min-width:30px;height:36px}\n"
".k15{flex:1.5;min-width:46px}\n"
".k2{flex:2;min-width:62px}\n"
".k225{flex:2.25;min-width:70px}\n"
".k275{flex:2.75;min-width:84px}\n"
".k6{flex:6;min-width:190px}\n"
".fn{background:#222;font-size:10px}\n"
".mod{background:#c22}\n"
".nav{background:#282828}\n"
".numpad{background:#1e3a1e}\n"
".media{background:#228822;font-size:13px}\n"
"</style>\n"
"</head>\n"
"<body>\n"
"<h3>ESP32 Wireless Keyboard</h3>\n"
"<div id=\"pad\"></div>\n"
"<textarea id=\"textInput\" placeholder=\"Use semicolon as separator:&#10;repeat(5){hi; delay(10)}&#10;repeat(3){Hello World; delay(500)}&#10;repeat(inf){[SPACE]; delay(100)}&#10;repeat(2){hi; delay(10); [ENTER]}\"></textarea>\n"
"<button id=\"sendBtn\">SEND</button>\n"
"<button id=\"clearBtn\">CLEAR</button>\n"
"<div class=\"keyboard\">\n"
"  <div class=\"row\">\n"
"    <div class=\"key k1 fn\" data-key=\"ESC\">Esc</div>\n"
"    <div style=\"flex:0.5\"></div>\n"
"    <div class=\"key k1 fn\" data-key=\"F1\">F1</div><div class=\"key k1 fn\" data-key=\"F2\">F2</div><div class=\"key k1 fn\" data-key=\"F3\">F3</div><div class=\"key k1 fn\" data-key=\"F4\">F4</div>\n"
"    <div style=\"flex:0.5\"></div>\n"
"    <div class=\"key k1 fn\" data-key=\"F5\">F5</div><div class=\"key k1 fn\" data-key=\"F6\">F6</div><div class=\"key k1 fn\" data-key=\"F7\">F7</div><div class=\"key k1 fn\" data-key=\"F8\">F8</div>\n"
"    <div style=\"flex:0.5\"></div>\n"
"    <div class=\"key k1 fn\" data-key=\"F9\">F9</div><div class=\"key k1 fn\" data-key=\"F10\">F10</div><div class=\"key k1 fn\" data-key=\"F11\">F11</div><div class=\"key k1 fn\" data-key=\"F12\">F12</div>\n"
"    <div class=\"key k15 nav\" data-key=\"PRTSC\">PrtSc</div>\n"
"    <div class=\"key k1 nav\" data-key=\"SCROLL\">ScrLk</div>\n"
"    <div class=\"key k1 nav\" data-key=\"PAUSE\">Pause</div>\n"
"  </div>\n"
"  <div class=\"row\">\n"
"    <div class=\"key k1\" data-key=\"`\">`</div>\n"
"    <div class=\"key k1\" data-key=\"1\">1</div><div class=\"key k1\" data-key=\"2\">2</div><div class=\"key k1\" data-key=\"3\">3</div><div class=\"key k1\" data-key=\"4\">4</div>\n"
"    <div class=\"key k1\" data-key=\"5\">5</div><div class=\"key k1\" data-key=\"6\">6</div><div class=\"key k1\" data-key=\"7\">7</div><div class=\"key k1\" data-key=\"8\">8</div>\n"
"    <div class=\"key k1\" data-key=\"9\">9</div><div class=\"key k1\" data-key=\"0\">0</div>\n"
"    <div class=\"key k1\" data-key=\"-\">-</div><div class=\"key k1\" data-key=\"=\">=</div>\n"
"    <div class=\"key k2\" data-key=\"BACKSPACE\">Backspace</div>\n"
"  </div>\n"
"  <div class=\"row\">\n"
"    <div class=\"key k15\" data-key=\"TAB\">Tab</div>\n"
"    <div class=\"key k1\" data-key=\"Q\">Q</div><div class=\"key k1\" data-key=\"W\">W</div><div class=\"key k1\" data-key=\"E\">E</div><div class=\"key k1\" data-key=\"R\">R</div>\n"
"    <div class=\"key k1\" data-key=\"T\">T</div><div class=\"key k1\" data-key=\"Y\">Y</div><div class=\"key k1\" data-key=\"U\">U</div><div class=\"key k1\" data-key=\"I\">I</div>\n"
"    <div class=\"key k1\" data-key=\"O\">O</div><div class=\"key k1\" data-key=\"P\">P</div>\n"
"    <div class=\"key k1\" data-key=\"[\">[</div><div class=\"key k1\" data-key=\"]\">]</div>\n"
"    <div class=\"key k15\" data-key=\"\\\\\">\\</div>\n"
"  </div>\n"
"  <div class=\"row\">\n"
"    <div class=\"key k2 mod\" data-key=\"CAPS\">Caps</div>\n"
"    <div class=\"key k1\" data-key=\"A\">A</div><div class=\"key k1\" data-key=\"S\">S</div><div class=\"key k1\" data-key=\"D\">D</div><div class=\"key k1\" data-key=\"F\">F</div>\n"
"    <div class=\"key k1\" data-key=\"G\">G</div><div class=\"key k1\" data-key=\"H\">H</div><div class=\"key k1\" data-key=\"J\">J</div><div class=\"key k1\" data-key=\"K\">K</div>\n"
"    <div class=\"key k1\" data-key=\"L\">L</div>\n"
"    <div class=\"key k1\" data-key=\";\">;</div><div class=\"key k1\" data-key=\"'\">'</div>\n"
"    <div class=\"key k225\" data-key=\"ENTER\">Enter</div>\n"
"  </div>\n"
"  <div class=\"row\">\n"
"    <div class=\"key k275 mod\" data-key=\"LSHIFT\">Shift</div>\n"
"    <div class=\"key k1\" data-key=\"Z\">Z</div><div class=\"key k1\" data-key=\"X\">X</div><div class=\"key k1\" data-key=\"C\">C</div><div class=\"key k1\" data-key=\"V\">V</div>\n"
"    <div class=\"key k1\" data-key=\"B\">B</div><div class=\"key k1\" data-key=\"N\">N</div><div class=\"key k1\" data-key=\"M\">M</div>\n"
"    <div class=\"key k1\" data-key=\",\">,</div><div class=\"key k1\" data-key=\".\">.</div><div class=\"key k1\" data-key=\"/\">/</div>\n"
"    <div class=\"key k275 mod\" data-key=\"RSHIFT\">Shift</div>\n"
"  </div>\n"
"  <div class=\"row\">\n"
"    <div class=\"key k15 mod\" data-key=\"LCTRL\">Ctrl</div>\n"
"    <div class=\"key k15 mod\" data-key=\"LALT\">Alt</div>\n"
"    <div class=\"key k1 mod\" data-key=\"LGUI\">Win</div>\n"
"    <div class=\"key k6\" data-key=\" \">&nbsp;</div>\n"
"    <div class=\"key k1 mod\" data-key=\"RGUI\">Win</div>\n"
"    <div class=\"key k15 mod\" data-key=\"RALT\">Alt</div>\n"
"    <div class=\"key k15 mod\" data-key=\"RCTRL\">Ctrl</div>\n"
"  </div>\n"
"  <div class=\"row\" style=\"margin-top:8px\">\n"
"    <div style=\"display:flex;flex-direction:column;gap:3px;margin-right:15px\">\n"
"      <div class=\"row\"><div class=\"key k1 nav\" data-key=\"INS\">Ins</div><div class=\"key k1 nav\" data-key=\"HOME\">Home</div><div class=\"key k1 nav\" data-key=\"PGUP\">PgUp</div></div>\n"
"      <div class=\"row\"><div class=\"key k1 nav\" data-key=\"DEL\">Del</div><div class=\"key k1 nav\" data-key=\"END\">End</div><div class=\"key k1 nav\" data-key=\"PGDN\">PgDn</div></div>\n"
"      <div class=\"row\"><div style=\"flex:1\"></div><div class=\"key k1 nav\" data-key=\"UP\">UP</div><div style=\"flex:1\"></div></div>\n"
"      <div class=\"row\"><div class=\"key k1 nav\" data-key=\"LEFT\">LEFT</div><div class=\"key k1 nav\" data-key=\"DOWN\">DOWN</div><div class=\"key k1 nav\" data-key=\"RIGHT\">RIGHT</div></div>\n"
"    </div>\n"
"    <div style=\"display:flex;flex-direction:column;gap:3px\">\n"
"      <div class=\"row\">\n"
"        <div class=\"key k1 numpad\" data-key=\"NUMLOCK\">Num</div>\n"
"        <div class=\"key k1 numpad\" data-key=\"/\">/</div>\n"
"        <div class=\"key k1 numpad\" data-key=\"*\">*</div>\n"
"        <div class=\"key k1 numpad\" data-key=\"-\">-</div>\n"
"      </div>\n"
"      <div class=\"row\">\n"
"        <div class=\"key k1 numpad\" data-key=\"7\">7</div><div class=\"key k1 numpad\" data-key=\"8\">8</div><div class=\"key k1 numpad\" data-key=\"9\">9</div>\n"
"        <div class=\"key k1 numpad\" data-key=\"+\">+</div>\n"
"      </div>\n"
"      <div class=\"row\">\n"
"        <div class=\"key k1 numpad\" data-key=\"4\">4</div><div class=\"key k1 numpad\" data-key=\"5\">5</div><div class=\"key k1 numpad\" data-key=\"6\">6</div>\n"
"      </div>\n"
"      <div class=\"row\">\n"
"        <div class=\"key k1 numpad\" data-key=\"1\">1</div><div class=\"key k1 numpad\" data-key=\"2\">2</div><div class=\"key k1 numpad\" data-key=\"3\">3</div>\n"
"        <div class=\"key k1 numpad\" style=\"height:76px;align-self:stretch\" data-key=\"ENTER\">Enter</div>\n"
"      </div>\n"
"      <div class=\"row\">\n"
"        <div class=\"key k2 numpad\" data-key=\"0\">0</div>\n"
"        <div class=\"key k1 numpad\" data-key=\".\">.</div>\n"
"      </div>\n"
"    </div>\n"
"  </div>\n"
"  <div class=\"row\" style=\"margin-top:12px;justify-content:center;gap:6px\">\n"
"    <div class=\"key media\" data-key=\"VOLUP\">Vol +</div>\n"
"    <div class=\"key media\" data-key=\"VOLDOWN\">Vol -</div>\n"
"    <div class=\"key media\" data-key=\"MUTE\">Mute</div>\n"
"    <div class=\"key media\" data-key=\"PLAY\">Play/Pause</div>\n"
"    <div class=\"key media\" data-key=\"NEXT\">Next</div>\n"
"    <div class=\"key media\" data-key=\"PREV\">Prev</div>\n"
"  </div>\n"
"</div>\n"
"<script>\n"
"// Touchpad with mouse support for both touchscreens and laptops\n"
"let mode='idle',lastX=0,lastY=0,startX=0,startY=0,moved=false,THRESHOLD=10;\n"
"const pad=document.getElementById('pad');\n"
"\n"
"function handleMoveStart(e) {\n"
"  e.preventDefault();\n"
"  const clientX = e.clientX ?? (e.touches ? e.touches[0].clientX : 0);\n"
"  const clientY = e.clientY ?? (e.touches ? e.touches[0].clientY : 0);\n"
"  mode='move';\n"
"  moved=false;\n"
"  startX=lastX=clientX;\n"
"  startY=lastY=clientY;\n"
"}\n"
"\n"
"function handleMove(e) {\n"
"  e.preventDefault();\n"
"  if(mode!=='move') return;\n"
"  let clientX, clientY;\n"
"  if(e.touches) { clientX=e.touches[0].clientX; clientY=e.touches[0].clientY; }\n"
"  else { clientX=e.clientX; clientY=e.clientY; }\n"
"  let dx=clientX-lastX, dy=clientY-lastY;\n"
"  if(Math.abs(clientX-startX)>THRESHOLD||Math.abs(clientY-startY)>THRESHOLD) moved=true;\n"
"  lastX=clientX; lastY=clientY;\n"
"  fetch(`/move?dx=${dx}&dy=${dy}`);\n"
"}\n"
"\n"
"function handleMoveEnd(e) {\n"
"  if(mode==='move' && !moved) {\n"
"    let rect=pad.getBoundingClientRect();\n"
"    let clickX = startX - rect.left;\n"
"    fetch(clickX<rect.width/2 ? '/cl' : '/cr');\n"
"  }\n"
"  mode='idle';\n"
"}\n"
"\n"
"pad.addEventListener('touchstart', handleMoveStart);\n"
"pad.addEventListener('touchmove', handleMove);\n"
"pad.addEventListener('touchend', handleMoveEnd);\n"
"pad.addEventListener('mousedown', handleMoveStart);\n"
"pad.addEventListener('mousemove', (e) => { if(mode==='move') handleMove(e); });\n"
"pad.addEventListener('mouseup', handleMoveEnd);\n"
"\n"
"// Scroll with two fingers (touch only) + mouse wheel\n"
"let scrollMode=false, lastScrollY=0;\n"
"pad.addEventListener('touchstart', (e)=>{\n"
"  if(e.touches.length===2) { scrollMode=true; lastScrollY=(e.touches[0].clientY+e.touches[1].clientY)/2; }\n"
"});\n"
"pad.addEventListener('touchmove', (e)=>{\n"
"  if(scrollMode && e.touches.length===2) {\n"
"    let y=(e.touches[0].clientY+e.touches[1].clientY)/2, dy=y-lastScrollY;\n"
"    lastScrollY=y;\n"
"    fetch(`/scroll?dy=${dy}`);\n"
"  }\n"
"});\n"
"pad.addEventListener('touchend', ()=>{ scrollMode=false; });\n"
"pad.addEventListener('wheel', (e)=>{\n"
"  fetch(`/scroll?dy=${e.deltaY/6}`);\n"
"});\n"
"\n"
"// Macro system (unchanged)\n"
"let textInput=document.getElementById('textInput');\n"
"let sendBtn=document.getElementById('sendBtn');\n"
"let clearBtn=document.getElementById('clearBtn');\n"
"let heldMods=new Set();\n"
"const modifiers=['LCTRL','LALT','LSHIFT','LGUI','RCTRL','RALT','RSHIFT','RGUI'];\n"
"const mediaKeys=['VOLUP','VOLDOWN','MUTE','PLAY','NEXT','PREV'];\n"
"\n"
"async function executeCommand(cmd) {\n"
"  cmd=cmd.trim();\n"
"  if(!cmd) return;\n"
"  if(cmd.toLowerCase().startsWith('delay(')) {\n"
"    let ms=parseInt(cmd.match(/\\d+/)||100);\n"
"    await new Promise(r=>setTimeout(r,ms));\n"
"  } else if(cmd.startsWith('[')) {\n"
"    let keys=cmd.match(/\\[(.*?)\\]/g).map(m=>m.slice(1,-1).trim().toUpperCase());\n"
"    await fetch(`/sendcombo?keys=${encodeURIComponent(keys.join(','))}`);\n"
"  } else {\n"
"    await fetch(`/type?text=${encodeURIComponent(cmd)}`);\n"
"  }\n"
"}\n"
"\n"
"async function executeRepeat(repeatCmd) {\n"
"  let openBrace=repeatCmd.indexOf('{');\n"
"  let closeBrace=repeatCmd.lastIndexOf('}');\n"
"  if(openBrace===-1||closeBrace===-1) return;\n"
"  let countStr=repeatCmd.substring(7,openBrace).trim();\n"
"  let inside=repeatCmd.substring(openBrace+1,closeBrace);\n"
"  let count=(countStr.toLowerCase()==='inf')?999999:parseInt(countStr);\n"
"  if(isNaN(count)) count=1;\n"
"  let commands=inside.split(';');\n"
"  for(let i=0;i<count;i++) {\n"
"    for(let cmd of commands) {\n"
"      await executeCommand(cmd);\n"
"      await new Promise(r=>setTimeout(r,15));\n"
"    }\n"
"  }\n"
"}\n"
"\n"
"async function executeMacro(text) {\n"
"  let i=0;\n"
"  while(i<text.length) {\n"
"    let lower=text.substring(i).toLowerCase();\n"
"    if(lower.startsWith('repeat(')) {\n"
"      let depth=0, start=i, j=i;\n"
"      for(;j<text.length;j++) {\n"
"        if(text[j]==='{') depth++;\n"
"        if(text[j]==='}') { depth--; if(depth===0) break; }\n"
"      }\n"
"      if(depth===0) {\n"
"        let repeatCmd=text.substring(start,j+1);\n"
"        await executeRepeat(repeatCmd);\n"
"        i=j+1;\n"
"        continue;\n"
"      }\n"
"    }\n"
"    let next=text.indexOf('repeat(',i);\n"
"    if(next===-1) next=text.length;\n"
"    let chunk=text.substring(i,next).trim();\n"
"    if(chunk) await executeCommand(chunk);\n"
"    i=next;\n"
"  }\n"
"}\n"
"\n"
"sendBtn.addEventListener('click', async ()=>{\n"
"  let text=textInput.value.trim();\n"
"  if(!text) return;\n"
"  sendBtn.disabled=true;\n"
"  sendBtn.textContent='SENDING...';\n"
"  await executeMacro(text);\n"
"  sendBtn.textContent='SEND';\n"
"  sendBtn.disabled=false;\n"
"});\n"
"clearBtn.addEventListener('click', ()=>textInput.value='');\n"
"\n"
"document.querySelectorAll('.key').forEach(k=>{\n"
"  const name=k.getAttribute('data-key');\n"
"  if(!name) return;\n"
"  k.addEventListener('click', ()=>{\n"
"    if(modifiers.includes(name)) {\n"
"      if(heldMods.has(name)) {\n"
"        heldMods.delete(name);\n"
"        k.classList.remove('held');\n"
"        fetch(`/release?key=${encodeURIComponent(name)}`);\n"
"      } else {\n"
"        heldMods.add(name);\n"
"        k.classList.add('held');\n"
"        fetch(`/press?key=${encodeURIComponent(name)}`);\n"
"      }\n"
"    } else if(mediaKeys.includes(name)) {\n"
"      fetch(`/special?key=${encodeURIComponent(name)}`);\n"
"    } else {\n"
"      const mods=Array.from(heldMods).join(',');\n"
"      fetch(`/combo?mods=${encodeURIComponent(mods)}&key=${encodeURIComponent(name)}`);\n"
"      heldMods.clear();\n"
"      document.querySelectorAll('.key.held').forEach(el=>el.classList.remove('held'));\n"
"    }\n"
"  });\n"
"});\n"
"</script>\n"
"</body>\n"
"</html>";

void pressKey(String k) {
  k.toUpperCase();
  if (k == "LCTRL" || k == "CTRL") Keyboard.press(KEY_LEFT_CTRL);
  else if (k == "RCTRL") Keyboard.press(KEY_RIGHT_CTRL);
  else if (k == "LALT" || k == "ALT") Keyboard.press(KEY_LEFT_ALT);
  else if (k == "RALT") Keyboard.press(KEY_RIGHT_ALT);
  else if (k == "LSHIFT" || k == "SHIFT") Keyboard.press(KEY_LEFT_SHIFT);
  else if (k == "RSHIFT") Keyboard.press(KEY_RIGHT_SHIFT);
  else if (k == "LGUI" || k == "WIN") Keyboard.press(KEY_LEFT_GUI);
  else if (k == "RGUI") Keyboard.press(KEY_RIGHT_GUI);
  else if (k == "ESC") Keyboard.press(KEY_ESC);
  else if (k == "ENTER" || k == "RETURN") Keyboard.press(KEY_RETURN);
  else if (k == "BACKSPACE") Keyboard.press(KEY_BACKSPACE);
  else if (k == "TAB") Keyboard.press(KEY_TAB);
  else if (k == "CAPS") Keyboard.press(KEY_CAPS_LOCK);
  else if (k == "DEL") Keyboard.press(KEY_DELETE);
  else if (k == "INS") Keyboard.press(KEY_INSERT);
  else if (k == "PRTSC") Keyboard.press(KEY_PRINT_SCREEN);
  else if (k == "SCROLL") Keyboard.press(KEY_SCROLL_LOCK);
  else if (k == "PAUSE") Keyboard.press(KEY_PAUSE);
  else if (k == "NUMLOCK") Keyboard.press(KEY_NUM_LOCK);
  else if (k == "UP") Keyboard.press(KEY_UP_ARROW);
  else if (k == "DOWN") Keyboard.press(KEY_DOWN_ARROW);
  else if (k == "LEFT") Keyboard.press(KEY_LEFT_ARROW);
  else if (k == "RIGHT") Keyboard.press(KEY_RIGHT_ARROW);
  else if (k == "HOME") Keyboard.press(KEY_HOME);
  else if (k == "END") Keyboard.press(KEY_END);
  else if (k == "PGUP") Keyboard.press(KEY_PAGE_UP);
  else if (k == "PGDN") Keyboard.press(KEY_PAGE_DOWN);
  else if (k.startsWith("F") && k.length() <= 3) {
    int n = k.substring(1).toInt();
    if (n >= 1 && n <= 12) Keyboard.press(KEY_F1 + n - 1);
  }
  else {
    Keyboard.print(k);
  }
}

void releaseKey(String k) {
  k.toUpperCase();
  if (k == "LCTRL" || k == "CTRL") Keyboard.release(KEY_LEFT_CTRL);
  else if (k == "RCTRL") Keyboard.release(KEY_RIGHT_CTRL);
  else if (k == "LALT" || k == "ALT") Keyboard.release(KEY_LEFT_ALT);
  else if (k == "RALT") Keyboard.release(KEY_RIGHT_ALT);
  else if (k == "LSHIFT" || k == "SHIFT") Keyboard.release(KEY_LEFT_SHIFT);
  else if (k == "RSHIFT") Keyboard.release(KEY_RIGHT_SHIFT);
  else if (k == "LGUI" || k == "WIN") Keyboard.release(KEY_LEFT_GUI);
  else if (k == "RGUI") Keyboard.release(KEY_RIGHT_GUI);
}

void handleRoot() { server.send(200, "text/html", webpage); }
void handleMove() { Mouse.move(server.arg("dx").toFloat(), server.arg("dy").toFloat()); server.send(200, "text/plain", "ok"); }
void handleScroll() { Mouse.move(0, 0, server.arg("dy").toInt() / 6); server.send(200, "text/plain", "ok"); }
void handleCL() { Mouse.click(MOUSE_LEFT); server.send(200, "text/plain", "ok"); }
void handleCR() { Mouse.click(MOUSE_RIGHT); server.send(200, "text/plain", "ok"); }

void handleType() {
  String text = server.arg("text");
  for (int i = 0; i < text.length(); i++) {
    char c = text[i];
    if (isAlpha(c) && isupper(c)) {
      Keyboard.press(KEY_LEFT_SHIFT);
      Keyboard.write(tolower(c)); 
      Keyboard.release(KEY_LEFT_SHIFT);
    } else {
      Keyboard.write(c);
    }
    delay(5);
  }
  server.send(200, "text/plain", "ok");
}

void handleSpecial() {
  String k = server.arg("key");
  k.toUpperCase();
  if (k == "VOLUP") { Consumer.press(0xE9); delay(30); Consumer.release(); }
  else if (k == "VOLDOWN") { Consumer.press(0xEA); delay(30); Consumer.release(); }
  else if (k == "MUTE") { Consumer.press(0xE2); delay(30); Consumer.release(); }
  else if (k == "PLAY") { Consumer.press(0xCD); delay(30); Consumer.release(); }
  else if (k == "NEXT") { Consumer.press(0xB5); delay(30); Consumer.release(); }
  else if (k == "PREV") { Consumer.press(0xB6); delay(30); Consumer.release(); }
  else { pressKey(k); delay(35); Keyboard.releaseAll(); }
  server.send(200, "text/plain", "ok");
}

void handlePress() { pressKey(server.arg("key")); server.send(200, "text/plain", "ok"); }
void handleRelease() { releaseKey(server.arg("key")); server.send(200, "text/plain", "ok"); }

void handleCombo() {
  String modsStr = server.arg("mods");
  String keyStr = server.arg("key");
  if (modsStr.length() > 0) {
    int start = 0;
    while (true) {
      int comma = modsStr.indexOf(',', start);
      String m = (comma == -1) ? modsStr.substring(start) : modsStr.substring(start, comma);
      if (m.length() > 0) pressKey(m);
      if (comma == -1) break;
      start = comma + 1;
    }
  }
  pressKey(keyStr);
  delay(35);
  Keyboard.releaseAll();
  server.send(200, "text/plain", "ok");
}

void handleSendCombo() {
  String keysStr = server.arg("keys");
  if (keysStr.length() > 0) {
    int start = 0;
    while (true) {
      int comma = keysStr.indexOf(',', start);
      String k = (comma == -1) ? keysStr.substring(start) : keysStr.substring(start, comma);
      if (k.length() > 0) pressKey(k);
      if (comma == -1) break;
      start = comma + 1;
    }
  }
  delay(35);
  Keyboard.releaseAll();
  server.send(200, "text/plain", "ok");
}

void setup() {
  Serial.begin(115200);
  USB.begin();
  delay(500);
  Mouse.begin();
  Keyboard.begin();
  Consumer.begin();
  
  WiFi.softAP(ssid, password);
  
  server.on("/", handleRoot);
  server.on("/move", handleMove);
  server.on("/scroll", handleScroll);
  server.on("/cl", handleCL);
  server.on("/cr", handleCR);
  server.on("/type", handleType);
  server.on("/special", handleSpecial);
  server.on("/press", handlePress);
  server.on("/release", handleRelease);
  server.on("/combo", handleCombo);
  server.on("/sendcombo", handleSendCombo);
  
  server.begin();
  Serial.println(WiFi.softAPIP());
}

void loop() {
  server.handleClient();
}