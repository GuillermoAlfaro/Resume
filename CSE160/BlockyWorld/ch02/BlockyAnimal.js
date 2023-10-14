// ColoredPoint.js (c) 2012 matsuda
// Vertex shader program
var VSHADER_SOURCE = `
precision mediump float;
attribute vec4 a_Position;

attribute vec2 a_UV;
attribute vec3 a_Normal;
varying vec2 v_UV;
varying vec3 v_Normal;
varying vec4 v_VertPos;

uniform mat4 u_ModalMatrix;
uniform mat4 u_ViewMatrix;
uniform mat4 u_ProjectionMatrix;
uniform mat4 u_RotationMatrix;
uniform mat4 u_NormalMatrix;

void main() {
  gl_Position = u_ProjectionMatrix * u_ViewMatrix * u_RotationMatrix * u_ModalMatrix * a_Position;
  v_UV = a_UV;
  v_Normal = a_Normal;
  v_VertPos = u_ModalMatrix * a_Position;
}`;

// Fragment shader program
var FSHADER_SOURCE = `
  precision mediump float;
  varying vec2 v_UV;
  varying vec3 v_Normal;
  uniform vec4 u_FragColor;
  uniform vec3 u_cameraPos;
  uniform sampler2D u_Sampler0;
  uniform sampler2D u_Sampler1;
  uniform sampler2D u_Sampler2;
  uniform int u_whichTexture;
  uniform vec3 u_lightPos;
  varying vec4 v_VertPos;
  uniform bool u_lightOn;

  void main() {

    if(u_whichTexture == -3){
        gl_FragColor = vec4((v_Normal+1.0)/2.0, 1.0);   // -3 Use Normal
    } else if (u_whichTexture == -2){
        gl_FragColor = u_FragColor;                     // -2 Use Color
    } else if (u_whichTexture == -1){
        gl_FragColor = vec4(v_UV, 1.0, 1.0);            // -1 Use UV debug color
    } else if (u_whichTexture == 0){
        gl_FragColor = texture2D(u_Sampler0, v_UV);     // 0 Use texture0
    } else if (u_whichTexture == 1) {
        gl_FragColor = texture2D(u_Sampler1, v_UV);     // 1 Use texture1
    } else if (u_whichTexture == 2){
        gl_FragColor = texture2D(u_Sampler2, v_UV);     // 2 Use texture2
    } else {
        gl_FragColor = vec4(1, .2, .3, 1);              // Error put Redish
    }

    vec3 lightVector = u_lightPos-vec3(v_VertPos);
    float r = length(lightVector);

    vec3 L = normalize(lightVector);
    vec3 N = normalize(v_Normal);
    float nDotL = max(dot(N, L), 0.0);

    // Reflection
    vec3 R = reflect(-L, N);

    // Eye
    vec3 E = normalize(u_cameraPos - vec3(v_VertPos));



    // Specular
    float specular = pow(max(dot(E, R), 0.01), 64.0);

    vec3 diffuse = vec3(1.0, 1.0, 0.9) * vec3(gl_FragColor) * nDotL * 0.7;
    vec3 ambient = vec3(gl_FragColor) * 0.2;

    
    if(u_lightOn){
        if (u_whichTexture >= -2){
            gl_FragColor = vec4(specular + diffuse + ambient, 1.0);
        } else {
            gl_FragColor = vec4(diffuse+ambient, 1.0);
        }
    }

    
  }`


// Global Varibles

let canvas;
let gl;
let a_Position;
let u_FragColor;
let u_CubeMatrix;
let a_UV;

let g_selectedColor = [1.0, 1.0, 1.0, 1.0];
let g_upperArmSlider = 0;
let g_lowerArmSlider = 0;

let g_right = 0;
let g_left = 0;
let g_head = 0;
let g_body = 0;

let g_rotateSlider = 0;
let g_lightPos = [0, 1, -2]
let u_cameraPos;
let g_lightOn = true;
let g_once = false;


var g_startTime = performance.now() / 1000
var g_seconds = performance.now() / 1000 - g_startTime
var g_creeperWalk = false;
var g_normal = false;                   // Lighting variable
var g_redEyes = false;
var g_camera = new Camera();

var rotationMatrix = new Matrix4();
var viewMatrix = new Matrix4();
var projectionMatrix = new Matrix4();
var normalMatrix = new Matrix4();
var u_Sampler0;
var u_Sampler1;
var u_Sampler2;
let u_lightPos;

var xMouse = 0;
var yMouse = 0;


function main() {
    setupWebGl();

    connectVariablesToGLSL();

    initTextures();

    addActions();

    gl.clearColor(0.0, 0.0, 0.0, 1.0);

    requestAnimationFrame(tick);

    document.onkeydown = keydown;

}

function tick() {

    g_seconds = performance.now() / 1000 - g_startTime

    updateAnimation();

    renderScene();

    requestAnimationFrame(tick);
}

function setupWebGl() {
    // Retrieve <canvas> element
    canvas = document.getElementById("webgl");

    // Get the rendering context for WebGL
    // gl = getWebGLContext(canvas);
    gl = canvas.getContext("webgl", { preserveDrawingBuffer: true });
    if (!gl) {
        console.log("Failed to get the rendering context for WebGL");
        return;
    }

    gl.enable(gl.DEPTH_TEST);
}

function connectVariablesToGLSL() {
    // Initialize shaders
    if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
        console.log("Failed to intialize shaders.");
        return;
    }

    // Get the storage location of a_Position
    a_Position = gl.getAttribLocation(gl.program, "a_Position");
    if (a_Position < 0) {
        console.log("Failed to get the storage location of a_Position");
        return;
    }

    a_UV = gl.getAttribLocation(gl.program, "a_UV");
    if (a_UV < 0) {
        console.log("Failed to get the storage location of a_UV");
        return;
    }

    a_Normal = gl.getAttribLocation(gl.program, "a_Normal");
    if (a_Normal < 0) {
        console.log("Failed to get the storage location of a_Normal");
        return;
    }

    // Get the storage location of u_FragColor
    u_FragColor = gl.getUniformLocation(gl.program, "u_FragColor");
    if (!u_FragColor) {
        console.log("Failed to get the storage location of u_FragColor");
        return;
    }

    u_ModalMatrix = gl.getUniformLocation(gl.program, "u_ModalMatrix");
    if (!u_ModalMatrix) {
        console.log("Failed to get the storage location of u_ModalMatrix");
    }

    u_ViewMatrix = gl.getUniformLocation(gl.program, "u_ViewMatrix");
    if (!u_ViewMatrix) {
        console.log("Failed to get the storage location of u_ViewMatrix");
    }

    u_NormalMatrix = gl.getUniformLocation(gl.program, "u_NormalMatrix");
    if (!u_NormalMatrix) {
        console.log("Failed to get the storage location of u_NormalMatrix");
    }

    u_ProjectionMatrix = gl.getUniformLocation(gl.program, "u_ProjectionMatrix");
    if (!u_ProjectionMatrix) {
        console.log("Failed to get the storage location of u_PerspectiveMatrix");
    }
    else {
        gl.uniformMatrix4fv(u_ProjectionMatrix, false, projectionMatrix.elements);
    }

    u_RotationMatrix = gl.getUniformLocation(gl.program, "u_RotationMatrix");
    if (!u_RotationMatrix) {
        console.log("Failed to get the storage location of u_RotationMatrix");
    }
    else {
        gl.uniformMatrix4fv(u_RotationMatrix, false, rotationMatrix.elements);
    }

    u_Sampler0 = gl.getUniformLocation(gl.program, 'u_Sampler0')
    if (!u_Sampler0) {
        console.log("Failed to create the u_Sampler0");
        return false
    }
    u_Sampler1 = gl.getUniformLocation(gl.program, 'u_Sampler1')
    if (!u_Sampler1) {
        console.log("Failed to create the u_Sampler1");
        return false
    }

    u_Sampler2 = gl.getUniformLocation(gl.program, 'u_Sampler2')
    if (!u_Sampler2) {
        console.log("Failed to create the u_Sampler2");
        return false
    }

    u_whichTexture = gl.getUniformLocation(gl.program, 'u_whichTexture')
    if (!u_whichTexture) {
        console.log("Failed to create the u_whichTexture");
        return false
    }

    u_lightPos = gl.getUniformLocation(gl.program, 'u_lightPos')
    if (!u_lightPos) {
        console.log("Failed to create the u_lightPos")
        return
    }

    u_cameraPos = gl.getUniformLocation(gl.program, 'u_cameraPos')
    if (!u_cameraPos) {
        console.log("Failed to create the u_cameraPos")
        return
    }

    u_lightOn = gl.getUniformLocation(gl.program, 'u_lightOn')
    if (!u_lightOn) {
        console.log("Failed to create the u_lightOn");
        return
    }

}

// Set up actions for the HTML UI elements
function addActions() {
    // Angle Sliders
    document.getElementById("upperArmSlider").addEventListener("mousemove", function () { g_upperArmSlider = this.value; });
    document.getElementById("lowerArmSlider").addEventListener("mousemove", function () { g_lowerArmSlider = this.value; });

    document.getElementById("lightX").addEventListener("mousemove", function (ev) { if (ev.buttons == 1) { g_lightPos[0] = this.value / 100; } })
    document.getElementById("lightY").addEventListener("mousemove", function (ev) { if (ev.buttons == 1) { g_lightPos[1] = this.value / 100; } })
    document.getElementById("lightZ").addEventListener("mousemove", function (ev) { if (ev.buttons == 1) { g_lightPos[2] = this.value / 100; } })

    document.getElementById("rotateSlider").addEventListener("mousemove", function () {
        g_rotateSlider = this.value;
        rotationMatrix.setRotate(g_rotateSlider, 0, 1, 0);
        gl.uniformMatrix4fv(u_RotationMatrix, false, rotationMatrix.elements)
    });

    // Buttons
    document.getElementById("creeperWalkOn").onclick = function () { g_creeperWalk = true; };
    document.getElementById("creeperWalkOff").onclick = function () { g_creeperWalk = false; };

    document.getElementById("normalOn").onclick = function () { g_normal = true; };
    document.getElementById("normalOff").onclick = function () { g_normal = false; };


    document.getElementById("lightOn").onclick = function () { g_lightOn = true; console.log(g_lightOn) };
    document.getElementById("lightOff").onclick = function () { g_lightOn = false; console.log(g_lightOn) };

    // Mouse Position
    document.getElementById("webgl").addEventListener('mousemove', function (event) {
        const rect = canvas.getBoundingClientRect();
        if (!event.shiftKey) {
            xMouse = event.clientX - rect.left - (rect.width / 2);
            yMouse = rect.height / 2 - (event.clientY - rect.top);
            moveView();
        }
    });

}

function moveView() {
    if (xMouse < 0) {
        g_camera.turnLeft(-xMouse * .01)
    } else if (xMouse > 0) {
        g_camera.turnRight(xMouse * .01)
    }

    if (yMouse < 0) {
        g_camera.lookDown(-yMouse * .01)
    } else if (yMouse > 0) {
        g_camera.lookUp(yMouse * .01)
    }
}

function keydown(ev) {

    if (ev.keyCode == 65) { // A
        g_camera.left()
        //console.log('A')
    } else if (ev.keyCode == 68) { // D
        g_camera.right();
        //console.log('D')
    } else if (ev.keyCode == 87) { // W
        g_camera.forward();
        //console.log('W')
    } else if (ev.keyCode == 83) { // S
        g_camera.back();
        //console.log('S')
    } else if (ev.keyCode == 81) { // Q or left arrow
        g_camera.turnLeft(5)
        //console.log('Q')
    } else if (ev.keyCode == 69) { // E or right arrow
        g_camera.turnRight(5)
        //console.log('E')
    } else if (ev.keyCode == 16) { // Shift
        g_camera.moveUp()
    } else if (ev.keyCode == 17) { // Control
        g_camera.moveDown()
    } else if (ev.keyCode == 32) { // Space
        g_camera.moveUp()
    }
}

function convertCoordsEventToGL(ev) {
    var x = ev.clientX; // x coordinate of a mouse pointer
    var y = ev.clientY; // y coordinate of a mouse pointer
    var rect = ev.target.getBoundingClientRect();

    x = (x - rect.left - canvas.width / 2) / (canvas.width / 2);
    y = (canvas.height / 2 - (y - rect.top)) / (canvas.height / 2);
    return [x, y];
}

function updateAnimation() {
    if (g_creeperWalk) {
        g_right = (35 * Math.sin(g_seconds));
        g_left = -(35 * Math.sin(g_seconds));
        g_head = -(15 * Math.sin(g_seconds));
        g_body = -(10 * Math.sin(g_seconds));
    }
    g_lightPos[0] = Math.cos(g_seconds)

}

function renderScene() {

    var startTime = performance.now()

    gl.clear(gl.COLOR_BUFFER_BIT | gl.DEPTH_BUFFER_BIT);

    gl.uniformMatrix4fv(u_ProjectionMatrix, false, g_camera.projectionMatrix.elements);
    gl.uniformMatrix4fv(u_ViewMatrix, false, g_camera.viewMatrix.elements)
    gl.uniform1i(u_lightOn, g_lightOn)


    var floor = new Cube();
    floor.color = [1, 0, 0, 1]
    floor.textureNum = 1;
    if (g_normal) floor.textureNum = -3
    floor.matrix.translate(0, -.75, 0.0);
    floor.matrix.scale(32, 0, 32);
    floor.matrix.translate(-.5, 0, -.5)
    floor.render();

    var skyBox = new Cube();
    skyBox.color = [1, 0, 0, 1]
    skyBox.textureNum = 0;
    skyBox.matrix.scale(-50, -50, -50);
    skyBox.matrix.translate(-.5, -.5, -.5)
    if (g_normal) skyBox.textureNum = -3
    skyBox.render()

    var front_right_foot = new Cube();
    front_right_foot.color = [92 / 255, 1, 92 / 255, 1];
    front_right_foot.matrix.translate(-.25, -.48, .1);
    front_right_foot.matrix.rotate(-180, 1, 0, 0)
    front_right_foot.matrix.rotate(g_right, 1, 0, 0)
    front_right_foot.matrix.scale(.2, .25, .25);
    front_right_foot.normalMatrix.setInverseOf(front_right_foot.matrix).transpose();
    front_right_foot.textureNum = -2
    if (g_normal) front_right_foot.textureNum = -3
    front_right_foot.render();

    var front_left_foot = new Cube();
    front_left_foot.color = [92 / 255, 1, 92 / 255, 1];
    front_left_foot.matrix.translate(-.05, -.48, .1);
    front_left_foot.matrix.rotate(-180, 1, 0, 0)
    front_left_foot.matrix.rotate(g_left, 1, 0, 0)
    front_left_foot.matrix.scale(.2, .25, .25);
    front_left_foot.textureNum = -2;
    if (g_normal) front_left_foot.textureNum = -3
    front_left_foot.render();

    var back_left_foot = new Cube();
    back_left_foot.color = [92 / 255, 1, 92 / 255, 1];
    back_left_foot.matrix.translate(-.05, -.48, .37);
    back_left_foot.matrix.rotate(90, 1, 0, 0)
    back_left_foot.matrix.rotate(g_left, 1, 0, 0)
    back_left_foot.matrix.scale(.2, .25, .25);
    back_left_foot.textureNum = -2
    if (g_normal) back_left_foot.textureNum = -3
    back_left_foot.render();

    var back_right_foot = new Cube();
    back_right_foot.color = [92 / 255, 1, 92 / 255, 1];
    back_right_foot.matrix.translate(-.25, -.48, .37);
    back_right_foot.matrix.rotate(90, 1, 0, 0)
    back_right_foot.matrix.rotate(g_right, 1, 0, 0)
    back_right_foot.matrix.scale(.2, .25, .25);
    back_right_foot.textureNum = -2
    if (g_normal) back_right_foot.textureNum = -3
    back_right_foot.render();

    var body = new Cube();
    body.color = [92 / 255, 1, 92 / 255, 1];
    body.matrix.setTranslate(-.05, -.5, 0.0);
    body.matrix.rotate(0, 1, 0, 0);
    body.matrix.rotate(0, 1, 0, 0);
    var bodyCoordsMat = new Matrix4(body.matrix);
    body.matrix.scale(0.4, 0.7, 0.3);
    body.matrix.translate(-.5, 0, .25);
    body.textureNum = -2
    if (g_normal) body.textureNum = -3
    body.render();


    var upperLeftArm = new Cube();
    upperLeftArm.color = [92 / 255, 1, 92 / 255, 1]
    upperLeftArm.matrix = new Matrix4(bodyCoordsMat);
    upperLeftArm.matrix.translate(-.1, .6, .3)
    upperLeftArm.matrix.rotate(90, 0, 1, 0)
    upperLeftArm.matrix.rotate(-180, 1, 0, 0)
    upperLeftArm.matrix.rotate(g_upperArmSlider, 1, 0, 0)
    var upperLefttArmCoordsMat = new Matrix4(upperLeftArm.matrix)
    upperLeftArm.matrix.scale(.1, .2, .2);
    upperLeftArm.textureNum = -2
    if (g_normal) upperLeftArm.textureNum = -3
    upperLeftArm.render();

    var lowerLeftArm = new Cube();
    lowerLeftArm.color = [92 / 255, 1, 92 / 255, 1]
    lowerLeftArm.matrix = new Matrix4(upperLefttArmCoordsMat);
    lowerLeftArm.matrix.translate(0, .2, 0)
    lowerLeftArm.matrix.rotate(g_lowerArmSlider, 1, 0, 0)
    lowerLeftArm.matrix.scale(.1, .2, .2);
    lowerLeftArm.textureNum = -2
    if (g_normal) lowerLeftArm.textureNum = -3
    lowerLeftArm.render();


    var upperRightArm = new Cube();
    upperRightArm.color = [92 / 255, 1, 92 / 255, 1]
    upperRightArm.matrix = new Matrix4(bodyCoordsMat)
    upperRightArm.matrix.translate(.1, .6, .2)
    upperRightArm.matrix.rotate(-90, 0, 0, 1);
    upperRightArm.matrix.rotate(Math.max(g_right, 0), 0, 0, 1);
    var upperRightArmCoordsMat = new Matrix4(upperRightArm.matrix)
    upperRightArm.matrix.scale(.2, .2, .1)
    upperRightArm.textureNum = -2
    if (g_normal) upperRightArm.textureNum = -3
    upperRightArm.render();

    var lowerRightArm = new Cube();
    lowerRightArm.color = [92 / 255, 1, 92 / 255, 1]
    lowerRightArm.matrix = new Matrix4(upperRightArmCoordsMat)
    lowerRightArm.matrix.translate(.2, 0, 0)
    lowerRightArm.matrix.rotate(0, 0, 0, 1);
    lowerRightArm.matrix.rotate(Math.max(g_right, 0), 0, 0, 1);
    lowerRightArm.matrix.scale(.2, .2, .1)
    lowerRightArm.textureNum = -2
    if (g_normal) lowerRightArm.textureNum = -3
    lowerRightArm.render();

    var head = new Cube();
    head.color = [92 / 255, 1, 92 / 255, 1]
    head.matrix = bodyCoordsMat
    head.matrix.translate(0, .65, 0)
    head.matrix.rotate(0, 0, 1, 0)
    head.matrix.scale(.41, .41, .41)
    head.matrix.translate(-.5, 0, -0.001)
    if (g_normal) head.textureNum = -3
    head.render();

    var left_eye = new Cube();
    if (g_redEyes) {
        left_eye.color = [1, 0, 0, 1]
    } else {
        left_eye.color = [0, 0, 0, 1]
    }
    left_eye.matrix = new Matrix4(head.matrix)
    left_eye.matrix.translate(0.12, .55, -0.001);
    left_eye.matrix.rotate(0, 0, 0, 1);
    left_eye.matrix.scale(.25, .25, .25);
    if (g_normal) left_eye.textureNum = -3
    left_eye.render()

    var right_eye = new Cube();
    if (g_redEyes) {
        right_eye.color = [1, 0, 0, 1]
    } else {
        right_eye.color = [0, 0, 0, 1]
    }
    right_eye.matrix = new Matrix4(head.matrix)
    right_eye.matrix.translate(0.64, .55, -0.001);
    right_eye.matrix.rotate(0, 0, 0, 1);
    right_eye.matrix.scale(.25, .25, .25);
    if (g_normal) right_eye.textureNum = -3
    right_eye.render()

    var top_mouth = new Cube();
    top_mouth.color = [0, 0, 0, 1]
    top_mouth.matrix = new Matrix4(head.matrix)
    top_mouth.matrix.translate(0.37, .4, -0.001);
    top_mouth.matrix.scale(.27, .15, .2);
    if (g_normal) top_mouth.textureNum = -3
    top_mouth.render()

    var open_mouth = new Cube();
    open_mouth.color = [0, 0, 0, 1]
    open_mouth.matrix = new Matrix4(head.matrix)
    open_mouth.matrix.translate(0.26, .15, -0.001);
    open_mouth.matrix.scale(.5, .25, .2);
    if (g_normal) open_mouth.textureNum = -3
    open_mouth.render()

    var left_mustache = new Cube();
    left_mustache.color = [0, 0, 0, 1]
    left_mustache.matrix = new Matrix4(head.matrix)
    left_mustache.matrix.translate(0.26, .06, -0.001);
    left_mustache.matrix.scale(.15, .15, .15);
    if (g_normal) left_mustache.textureNum = -3
    left_mustache.render()

    var right_mustache = new Cube();
    right_mustache.color = [0, 0, 0, 1]
    right_mustache.matrix = new Matrix4(head.matrix)
    right_mustache.matrix.translate(0.61, .06, -0.001);
    right_mustache.matrix.scale(.15, .15, .15);
    if (g_normal) right_mustache.textureNum = -3
    right_mustache.render()

    drawMap();

    gl.uniform3f(u_lightPos, g_lightPos[0], g_lightPos[1], g_lightPos[2])

    gl.uniform3f(u_cameraPos, g_camera.eye.elements[0], g_camera.eye.elements[1], g_camera.eye.elements[2])

    var lightBox = new Cube();
    lightBox.color = [2, 2, 0, 1]
    lightBox.matrix.translate(g_lightPos[0], g_lightPos[1], g_lightPos[2])
    lightBox.matrix.scale(-.1, -.1, -.1)
    lightBox.matrix.translate(-.5, -.5, -.5)
    lightBox.render()

    var sun = new Sphere();
    sun.matrix.translate(0, 1, 2);
    if (g_normal) sun.textureNum = -3
    sun.render();

    var duration = performance.now() - startTime

    sendTextToHTML(duration)
}

function initTextures() {

    var skyImg = new Image();
    var groundImg = new Image();
    var wallImg = new Image();
    if (!skyImg) {
        console.log("Failed to create the skyImg");
        return false
    }
    if (!groundImg) {
        console.log("Failed to create the groundImg");
        return false
    }
    if (!wallImg) {
        console.log("Failed to create the wallImg");
        return false
    }

    skyImg.onload = function () { sendImagetoTEXTURE0(skyImg); };
    groundImg.onload = function () { sendImagetoTEXTURE1(groundImg); };
    wallImg.onload = function () { sendImagetoTEXTURE2(wallImg); };

    skyImg.src = 'sky.jpg';
    groundImg.src = 'grass.jpeg'
    wallImg.src = 'cobblestone.png'

    return true;
}

function sendImagetoTEXTURE0(image) {

    var texture = gl.createTexture();                   // Create a texture on the GPU
    if (!texture) {
        console.log("Failed to create the texture object");
        return false;
    }

    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);          // flip the image'y y axis

    gl.activeTexture(gl.TEXTURE0);                      // Actiavate texture unit 0

    gl.bindTexture(gl.TEXTURE_2D, texture);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);

    gl.uniform1i(u_Sampler0, 0);

    console.log("finished sendImagetoTEXTURE0")
}

function sendImagetoTEXTURE1(image) {

    var texture = gl.createTexture();                   // Create a texture on the GPU
    if (!texture) {
        console.log("Failed to create the texture object");
        return false;
    }

    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);          // flip the image'y y axis

    gl.activeTexture(gl.TEXTURE1);                      // Actiavate texture unit 0

    gl.bindTexture(gl.TEXTURE_2D, texture);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);

    gl.uniform1i(u_Sampler1, 1);

    console.log("finished sendImagetoTEXTURE1")
}

function sendImagetoTEXTURE2(image) {

    var texture = gl.createTexture();                   // Create a texture on the GPU
    if (!texture) {
        console.log("Failed to create the texture2 object");
        return false;
    }

    gl.pixelStorei(gl.UNPACK_FLIP_Y_WEBGL, 1);          // flip the image'y y axis

    gl.activeTexture(gl.TEXTURE2);                      // Actiavate texture unit 0

    gl.bindTexture(gl.TEXTURE_2D, texture);

    gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.LINEAR);

    gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGB, gl.RGB, gl.UNSIGNED_BYTE, image);

    gl.uniform1i(u_Sampler2, 2);

    console.log("finished sendImagetoTEXTURE1")
}

function sendTextToHTML(duration) {
    var htmlElm = document.getElementById("numdot");

    if (!htmlElm) {
        console.log("Failed to get " + htmlID + " from HTML");
        return;
    }

    htmlElm.innerHTML = "ms: " + Math.floor(duration) + " fps: " + Math.floor(100 / duration)
}

function drawMap() {
    var body = new Cube();
    for (x = 0; x < 32; x++) {
        for (y = 0; y < 32; y++) {
            if (x == 0 || y == 0 || x == 31 || y == 31) {
                body.color = [1, 1, 1, 1];
                body.matrix.translate(x - 16, -.75, y - 16);
                body.textureNum = 2;
                body.matrix.scale(1, 1, 1);
                body.render()
                body.matrix.setIdentity();
            }
        }
    }

}