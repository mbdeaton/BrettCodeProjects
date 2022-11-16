// Tools to draw a logarithmic spiral representing all possible
// notes as an SVG. Prototype by Brett Deaton - Fall 2022

// Draw the background disc enclosing all the notes within radius.
function drawMainDisc(radius) {
  let mainCirc = document.createElementNS(svgns, "circle");
  mainCirc.setAttribute("cx", "0");
  mainCirc.setAttribute("cy", "0");
  mainCirc.setAttribute("r", `${radiusFundamental}`);
  mainCirc.setAttribute("fill", "lightblue");
  svg.appendChild(mainCirc);
}

// Draw 12 radii representing the 12 notes of the chromatic scale.
function drawNoteLines() {
  const noteNames = [
    "C",
    "C#",
    "D",
    "D#",
    "E",
    "F",
    "F#",
    "G",
    "G#",
    "A",
    "A#",
    "B",
  ];
  const angleStep = (2 * Math.PI) / 12;

  for (let i = 0; i < noteNames.length; i++) {
    const angle = i * angleStep + Math.PI / 2;
    const x = -radiusFundamental * Math.cos(angle);
    const y = -radiusFundamental * Math.sin(angle);

    let noteLine = document.createElementNS(svgns, "line");
    noteLine.classList.add("note-line");
    noteLine.setAttribute("x2", `${x}`);
    noteLine.setAttribute("y2", `${y}`);
    noteLine.setAttribute("stroke", "whitesmoke");
    noteLine.setAttribute("stroke-width", "0.2px");

    let noteLabel = document.createElementNS(svgns, "text");
    noteLabel.setAttribute("x", `${1.05 * x}`);
    noteLabel.setAttribute("y", `${1.05 * y}`);
    noteLabel.setAttribute("fill", "lightblue");
    noteLabel.setAttribute("font-size", "0.2rem");
    noteLabel.setAttribute("text-anchor", "middle");
    noteLabel.setAttribute("dominant-baseline", "middle");
    noteLabel.innerHTML = noteNames[i];

    svg.appendChild(noteLine);
    svg.appendChild(noteLabel);
  }
}

// Draw the logarithmic spiral representing all frequencies.
//   r = r_0 exp(b(theta-pi/2))
// where
//   r_0    the fundamental radius, representing the key center
//   r      coordinate radius in SVG units, proportional the wavelength
//   theta  counter-clockwise angle from positive y-axis (in the math
//          convention, going up the page), with larger theta representing
//          lower notes.
//   b      rate constant forcing the spiral to double in radius
//          every full rotation
// Details on this equation in `notes/notes-log_spiral-?.png`.
function drawSpiral() {
  const numOctaves = 8; // how many octaves to display?
  const centsPerStep = 25; // decrease for better spiral resolution
  const b = Math.LN2 / 2 / Math.PI;
  const centsPerOctave = 1200;
  const numSteps = (numOctaves * centsPerOctave) / centsPerStep;
  const angleStep = (2 * Math.PI * centsPerStep) / centsPerOctave;
  const radiusStart = radiusFundamental / 2 ** numOctaves;
  let spiral = document.createElementNS(svgns, "path");
  spiral.id = "spiral";
  spiral.setAttribute("stroke", "gray");
  spiral.setAttribute("stroke-width", "0.2px");
  spiral.setAttribute("fill", "none");

  const pointPairsXY = [0, -radiusStart];
  for (let i = 1; i < numSteps + 1; i++) {
    const angle = i * angleStep + Math.PI / 2;
    const radius = radiusStart * Math.exp(b * (angle - Math.PI / 2));
    const x = radius * Math.cos(angle);
    const y = -radius * Math.sin(angle);
    pointPairsXY.push(x, y);
  }

  spiral.setAttribute("d", "M" + pointPairsXY.join(" "));
  svg.appendChild(spiral);
}

// Create the empty svg group containing the notes
function insertNoteGroup() {
  let noteGroup = document.createElementNS(svgns, "g");
  noteGroup.id = "notes";
  svg.appendChild(noteGroup);
}

// Play a note of given frequency (Hz) and duration (s).
function playNote(frequency, duration = 0.5) {
  // create Oscillator node
  const oscillator = audioCtx.createOscillator();

  oscillator.frequency.value = frequency;
  oscillator.connect(audioCtx.destination);
  oscillator.start(0);
  oscillator.stop(audioCtx.currentTime + duration);
}

// Return the unique x,y pair on the spiral having radius.
//   theta = log(r/r_0)/b + c
// where
//   theta  counter-clockwise angle from positive y-axis (in the math
//          convention, going up the page), with larger theta representing
//          lower notes.
//   r      coordinate radius in SVG units, proportional the wavelength
//   r_0    the fundamental radius, representing the key center
//   b      rate constant forcing the spiral to double in radius
//          every full rotation
// Details on this equation in `notes-log_spiral-?.png`.
function coordsFromRadius(radius) {
  const b = 1 / Math.LOG2E / 2 / Math.PI;
  const c = Math.PI / 2;
  const theta = Math.log(radius / radiusFundamental) / b + c;
  return [radius * Math.cos(theta), -radius * Math.sin(theta)]; // [x,y]
}

// Given a radius, return the corresponding frequency, or vice versa.
// The physics relationship is frequency = soundspeed/2pi/radius
function frequencyFromRadius(radius) {
  return scaleSpeed / radius; // in Hz
}
function radiusFromFrequency(frequency) {
  return scaleSpeed / frequency; // in SVG units
}

// Draw a dot on the spiral at the given radius.
function drawNote(radius) {
  const radiusDot = radiusFundamental / 50;
  const [x, y] = coordsFromRadius(radius);
  let dot = document.createElementNS(svgns, "circle");
  dot.setAttribute("cx", `${x}`);
  dot.setAttribute("cy", `${y}`);
  dot.setAttribute("r", `${radiusDot}`);
  dot.setAttribute("fill", "orange");
  noteGroup.appendChild(dot);
}

// Return the matrix M that translates DOM coords x to SVG coords x' using
// x' = x.matrixTransform(M)
function getDomMatrix() {
  rectCanvas = document.getElementById("spiral-canvas").getBoundingClientRect();

  // get translation
  const translateX = rectCanvas.x;
  const translateY = rectCanvas.y;
  // console.log(`translation vector: (${translateX}, ${translateY})`); // DEBUG

  // get scale
  const scale = scaleSvg / rectCanvas.width;
  // console.log(`width: ${rectCanvas.width}`); // DEBUG
  // console.log(`scale: ${scale}`); // DEBUG

  // create matrix
  // matrix components: https://developer.mozilla.org/en-US/docs/Web/API/DOMMatrix/DOMMatrix
  // and https://drafts.fxtf.org/geometry/#matrix
  // Matrix transformations (M.blahSelf()) transform x coord system (in this
  // case DOM) wrt to the x' coord system (in this case SVG), in the order
  // applied. Then the operation x' = x.matrixTransform(M) gives you the
  // coordinates of the point in the x' coord system.
  let matrix = new DOMMatrix();
  matrix.translateSelf(-scaleSvg / 2, -scaleSvg / 2);
  matrix.scaleSelf(scaleSvg / rectCanvas.width);
  matrix.translateSelf(-rectCanvas.x, -rectCanvas.y);
  return matrix;
}

function handleClick(evnt) {
  const matrix = getDomMatrix();
  const point = new DOMPoint(evnt.clientX, evnt.clientY);
  // console.log(`point: ${point.x} ${point.y}`); // DEBUG
  const pointSvg = point.matrixTransform(matrix);
  // console.log(`pointSvg: ${pointSvg.x} ${pointSvg.y}`); // DEBUG

  const x = pointSvg.x;
  const y = pointSvg.y;
  const r = Math.sqrt(x ** 2 + y ** 2);
  drawNote(r);
  playNote(frequencyFromRadius(r));
}

// Main driver
const c = document.getElementById("spiral-canvas");
const svg = document.querySelector("svg");
const svgns = "http://www.w3.org/2000/svg";
const scaleSvg = 100; // SVG unit size of the viewBox
const radiusFundamental = (0.9 * scaleSvg) / 2;
c.setAttribute(
  "viewBox",
  `${-scaleSvg / 2} ${-scaleSvg / 2} ${scaleSvg} ${scaleSvg}`
);
const audioCtx = new (window.AudioContext || window.webkitAudioContext)();
svg.addEventListener("click", handleClick);

// Set scale between lengths and frequencies
const frequencyC4 = 261.626; // middle C, i.e. C4
const frequencyFundamental = frequencyC4 / 2; // lowest note
const scaleSpeed = frequencyFundamental * radiusFundamental;

drawMainDisc();
drawNoteLines();
drawSpiral();
insertNoteGroup();
const noteGroup = document.getElementById("notes");
