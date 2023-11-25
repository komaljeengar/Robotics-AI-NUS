/* 
 *  C08 - p5.js WebSocket LED with Teachable Machine 
 *         Tensorflow.js model support.
 *  Creating a fancy interactive user interface 
 *  capable of communicating via WebSocket in p5.js
 *  
 *  
 */

const modelURL = 'https://teachablemachine.withgoogle.com/models/XXXX-XXXX/';

var classifier;
var video;
var flippedVideo;
var label = "";
var previousLabel;

var leftMove = false;
var rightMove = false;

function preload() {
    classifier = ml5.imageClassifier(modelURL + 'model.json');
}

function setup() {
    createCanvas(windowWidth,windowHeight);
  
    video = createCapture(VIDEO);
    video.size(320, 240);
    video.hide();
    flippedVideo = ml5.flipImage(video);
    classifyVideo();
}

function draw() {
    background(0);
    image(flippedVideo, 10, 10);
    fill(255);
    textSize(16);
    textAlign(CENTER);
    text(label == ""? "I see nothing!":"I see " + label, width / 2, 100);
  
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

function classifyVideo() {
    flippedVideo = ml5.flipImage(video)
    classifier.classify(flippedVideo, gotResult);
    flippedVideo.remove();
}

function gotResult(error, results) {
    if (error) {
        console.error(error);
        return;
    }
    label = String(results[0].label);
    console.log(label);
  
    if ((label != "Background") && (previousLabel != label)) {
        if (label == "Robot_Go") {
            leftMove = true;
            rightMove = true;  
        }
        else if (label == "Robot_Stop") {
            leftMove = false;
            rightMove = false;           
        }
    }
    previousLabel = label;
  
    classifyVideo();
}

                    
                
