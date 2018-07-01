var milkcocoa = new MilkCocoa('');
var chatDataStore = milkcocoa.dataStore("");
var board;
window.onload = function(){
  board = document.getElementById("board");
}
 
function clickEvent_one(){
  var v_r = 1; var v_g = 1; var v_b = 1; var v_s = 1; var v_t = 1;
  sendText(v_r, v_g, v_b, v_s, v_t);
}

function clickEvent_two(){
  var v_r = 1; var v_g = 1; var v_b = 1; var v_s = 1; var v_t = 10;
  sendText(v_r, v_g, v_b, v_s, v_t);
}

function sendText(v_r, v_g, v_b, v_s, v_t){
  chatDataStore.push({r : v_r, g : v_g, b : v_b, s : v_s, t : v_t},function(data){
    console.log("送信完了!");
  });
}
 
chatDataStore.on("push",function(data){
  addText(data.value.r, data.value.g, data.value.b, data.value.s, data.value.t);
});
 
function addText(v_r, v_g, v_b, v_s, v_t){
  var msgDom = document.createElement("li");
  msgDom.innerHTML = v_r + "," + v_g + "," + v_b + "," + v_s + "," + v_t;
  board.insertBefore(msgDom, board.firstChild);
}

