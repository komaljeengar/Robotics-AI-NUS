                    
/* 
 *  C06 - p5.js Screen Control Simple Robot
 *  Creating a fancy interactive user interface 
 *  capable of communicating via the Browser/Phone's Screen
 *  in p5.js
 *  
 */

var leftMove = false;
var rightMove = false;
var goButton;

var ButtonSize = 200;

function setup() {
  createCanvas(windowWidth,windowHeight);
  
  goButton = createButton('GO').size(ButtonSize, ButtonSize/2);
  goButton.position(((windowWidth - ButtonSize) /2) , 100);
  goButton.style('font-size', '30px');
  goButton.style('background-color', color('green'));
  goButton.mousePressed(()  => {
                                  leftMove = true; 
                                  rightMove = true; 
                                  setTimeout(resetMove, 500);
                                })
  // myButton.mouseReleased(() => {leftMove = false; rightMove = false;});
}

function resetMove() {
  leftMove = false; 
  rightMove = false;
}

function draw() {
  background(0);
  stroke('red');
  
  if (leftMove) {
      fill(255);     
      strokeWeight(0); 
  }
  else {
      fill(0);
      strokeWeight(1);
  }
  // Draw a rectangle at (x, y, width, height , rounded corners).
  rect(10, windowHeight - 250 , (windowWidth/2) - 20, 200, 20);
  
  if (rightMove) {
      fill(255);     
      strokeWeight(0); 
  }
  else {
      fill(0);
      strokeWeight(1);
  }
  // Draw a rectangle at (x, y, width, height , rounded corners).
  rect((windowWidth/2) + 10, windowHeight - 250 , (windowWidth/2) - 20, 200, 20);
}
                    
                
