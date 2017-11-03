// Karen Lee
// kglee
// lab1.js
// Creates polylines on left mouse clicks and clears the canvas
// on right mouse clicks

// Vertex shader program
var VSHADER_SOURCE =
   'attribute vec4 a_Position;\n' +
   'void main() {\n' +
   '  gl_Position = a_Position;\n' +
   '  gl_PointSize = 10.0;\n' +
   '}\n';

// Fragment shader program
var FSHADER_SOURCE =
   'void main() {\n' +
   '  gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n' +
   '}\n';

function main() {
   // Retrieve <canvas> element
   var canvas = document.getElementById('webgl');

   // Get the rendering context for WebGL
   var gl = getWebGLContext(canvas);
   if (!gl) {
      console.log('Failed to get the rendering context for WebGL');
      return;
   }

   // Initialize shaders
   if (!initShaders(gl, VSHADER_SOURCE, FSHADER_SOURCE)) {
      console.log('Failed to intialize shaders.');
      return;
   }
  
   // Create a buffer object
   var vertexBuffer = gl.createBuffer();
   if (!vertexBuffer) {
      console.log('Failed to create the buffer object');
      return -1;
   }

   // Get the storage location of a_Position
   var a_Position = gl.getAttribLocation(gl.program, 'a_Position');
   if (a_Position < 0) {
      console.log('Failed to get the storage location of a_Position');
      return;
   } 

   // Register function (event handler) to be called on a mouse press
   canvas.onmousedown = function(ev){ click(ev, gl, canvas, a_Position, vertexBuffer); };
  
   // Register function (event handler) to be called on mouse move over
   canvas.onmousemove = function(ev){ move(ev, gl, canvas, a_Position, vertexBuffer); };

   // Specify the color for clearing <canvas>
   gl.clearColor(0.0, 0.0, 0.0, 1.0);

   // Clear <canvas>
   gl.clear(gl.COLOR_BUFFER_BIT);
  
}

var g_points = []; // The array for the position of a mouse press
var vertices = []; // Array of vertices
var count = 0; // vertex count
var len = 0;  // length of g_points array
function click(ev, gl, canvas, a_Position, vertexBuffer) {
   var x = ev.clientX; // x coordinate of a mouse pointer
   var y = ev.clientY; // y coordinate of a mouse pointer
   var rect = ev.target.getBoundingClientRect() ;
   // If right mouse button is clicked
   if (ev.which == 3 || ev.button == 2) {
      console.log("Right mouse click: (" + x + ", " + y + ")");
	  
	  var n = 0;
	  // Print out vertices list to console
      for (var i = 0; i < g_points.length-2; i+=2) {
	     console.log("Vertex " + (n++) + ": (" + vertices[i] + ", " +
		              vertices[i+1] + ")");
	  }
	  
	  // Clear canvas and arrays
	  gl.clear(gl.COLOR_BUFFER_BIT);
	  g_points = [];
	  vertices = [];
	  count = 0;

   } else { // Left mouse button is clicked	   
      console.log("Left mouse click: (" + x + ", " + y + ")");

	  vertices.push(x); vertices.push(y);
      x = ((x - rect.left) - canvas.width/2)/(canvas.width/2);
      y = (canvas.height/2 - (y - rect.top))/(canvas.height/2);
      // Store the coordinates to g_points array
      g_points.push(x); g_points.push(y);

      // Clear <canvas>
      gl.clear(gl.COLOR_BUFFER_BIT);
  
      len = g_points.length;
      count = len/2;    
  
      // Bind the buffer object to target
      gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	
      // Pass the vertex data to the buffer
      gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(g_points), gl.STATIC_DRAW);
	
      // Assign the buffer object to a_Position variable
      gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);

      // Enable the assignment to a_Position variable
      gl.enableVertexAttribArray(a_Position);
  
      // Draw points
      gl.drawArrays(gl.POINTS, 0, count);

      if (count > 1) {
         // Draw lines
         gl.drawArrays(gl.LINE_STRIP, 0, count);
      }
   }
   //document.getElementById("debug").innerHTML = "Vertices: " + count;
  
}

function move(ev, gl, canvas, a_Position, vertexBuffer) {
   var x = ev.clientX; // x coordinate of a mouse pointer
   var y = ev.clientY; // y coordinate of a mouse pointer
   var rect = ev.target.getBoundingClientRect() ;
  
   x = ((x - rect.left) - canvas.width/2)/(canvas.width/2);
   y = (canvas.height/2 - (y - rect.top))/(canvas.height/2);
  
   var len = g_points.length;
   count = len/2;
  
   // Make the rubberbanding vertex after the first vertex is made
   if (count == 1) {
      g_points.push(x); g_points.push(y);
   }
   // Change position of the rubberbanding vertex
   if (count > 1) {
      g_points[len-2] = x;
      g_points[len-1] = y;
   }
  
   // Clear <canvas>
   gl.clear(gl.COLOR_BUFFER_BIT);
  
   // Bind the buffer object to target
   gl.bindBuffer(gl.ARRAY_BUFFER, vertexBuffer);
	
   // Pass the vertex data to the buffer
   gl.bufferData(gl.ARRAY_BUFFER, new Float32Array(g_points), gl.STATIC_DRAW);
	
   // Assign the buffer object to a_Position variable
   gl.vertexAttribPointer(a_Position, 2, gl.FLOAT, false, 0, 0);

   // Enable the assignment to a_Position variable
   gl.enableVertexAttribArray(a_Position);
  
   if (count > 1) {
      gl.drawArrays(gl.POINTS, 0, count);
      gl.drawArrays(gl.LINE_STRIP, 0, count);
   }
  
}
