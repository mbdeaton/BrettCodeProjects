const c = document.getElementById("spiral-canvas");
const svg = document.querySelector("svg");
const svgns = "http://www.w3.org/2000/svg";
const scale = 100;
const radius = 0.9*scale/2;
c.setAttribute("viewBox", `${-scale/2} ${-scale/2} ${scale} ${scale}`);
let mainCirc = document.createElementNS(svgns, "circle");
mainCirc.setAttribute("cx", "0");
mainCirc.setAttribute("cy", "0");
mainCirc.setAttribute("r", `${radius}`);
mainCirc.setAttribute("fill", "lightblue");
svg.appendChild(mainCirc);
const audioCtx = new (window.AudioContext || window.webkitAudioContext)();

function playNote() {
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
  const noteNames = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];
  const angleStep = 2*Math.PI / 12;
  
  for (let i = 0; i < noteNames.length; i++) {
    const angle = i*angleStep + Math.PI/2;
    const x = -radius*Math.cos(angle);
    const y = -radius*Math.sin(angle);

    let noteLine = document.createElementNS(svgns, "line");
    noteLine.classList.add("note-line");
    noteLine.setAttribute("x2", `${x}`);
    noteLine.setAttribute("y2", `${y}`);
    noteLine.setAttribute("stroke", "whitesmoke");
    noteLine.setAttribute("stroke-width", "0.2px");

    let noteLabel = document.createElementNS(svgns, "text");
    noteLabel.setAttribute("x", `${1.05*x}`);
    noteLabel.setAttribute("y", `${1.05*y}`);
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
  const numSteps = numOctaves * 1200 / centsPerStep;
  const angleStep = 2*Math.PI * centsPerStep / 1200;
  const radiusStart = scale/400;
  let spiral = document.createElementNS(svgns, "path");
  spiral.id = "spiral";
  spiral.setAttribute("stroke", "gray");
  spiral.setAttribute("stroke-width", "0.2px");
  spiral.setAttribute("fill", "none");

  const pointPairsXY = [0, -radiusStart];
  for (let i=1; i < numSteps+1; i++) {
    const angle = i*angleStep + Math.PI/2;
    const radius = radiusStart * Math.exp(b*(angle-Math.PI/2));
    const x = radius*Math.cos(angle);
    const y = -radius*Math.sin(angle);
    pointPairsXY.push(x, y);
  }

  spiral.setAttribute("d", "M" + pointPairsXY.join(" "));
  svg.appendChild(spiral);
}

drawNoteLines();
drawSpiral();
