const c = document.getElementById("spiral-canvas");
const svg = document.querySelector("svg");
const svgns = "http://www.w3.org/2000/svg";
const scaleSvg = 100;
const radius = (0.9 * scaleSvg) / 2;
c.setAttribute(
  "viewBox",
  `${-scaleSvg / 2} ${-scaleSvg / 2} ${scaleSvg} ${scaleSvg}`
);
let mainCirc = document.createElementNS(svgns, "circle");
mainCirc.setAttribute("cx", "0");
mainCirc.setAttribute("cy", "0");
mainCirc.setAttribute("r", `${radius}`);
mainCirc.setAttribute("fill", "lightblue");
svg.appendChild(mainCirc);
const audioCtx = new (window.AudioContext || window.webkitAudioContext)();

svg.addEventListener("click", playNote);

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

function playNote(evnt) {
  const matrix = getDomMatrix();
  const point = new DOMPoint(evnt.clientX, evnt.clientY);
  // console.log(`point: ${point.x} ${point.y}`); // DEBUG
  const pointSvg = point.matrixTransform(matrix);
  // console.log(`pointSvg: ${pointSvg.x} ${pointSvg.y}`); // DEBUG

  const frequency = 261.626; // middle C, i.e. C4
  const duration = 0.5;

  // create Oscillator node
  const oscillator = audioCtx.createOscillator();

  oscillator.frequency.value = frequency;
  oscillator.connect(audioCtx.destination);
  oscillator.start(0);
  oscillator.stop(audioCtx.currentTime + duration);
}

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
    const x = -radius * Math.cos(angle);
    const y = -radius * Math.sin(angle);

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

function drawSpiral() {
  const b = Math.LN2 / 2 / Math.PI;
  const centsPerStep = 25;
  const numOctaves = 8;
  const numSteps = (numOctaves * 1200) / centsPerStep;
  const angleStep = (2 * Math.PI * centsPerStep) / 1200;
  const radiusStart = scaleSvg / 400;
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

drawNoteLines();
drawSpiral();
