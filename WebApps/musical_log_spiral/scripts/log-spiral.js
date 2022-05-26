const c = document.getElementById("spiral-canvas");
const scale = 100;
const radius = 0.9*scale/2;
c.setAttribute("viewBox", `${-scale/2} ${-scale/2} ${scale} ${scale}`)
c.innerHTML += `<circle cx='0' cy='0' r='${radius}' fill='lightblue'/>`;

function drawNoteLines() {
  const noteNames = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];
  const angleStep = 2*Math.PI / 12;
  
  for (let i = 0; i < noteNames.length; i++) {
    const angle = i*angleStep + Math.PI/2;
    const x = -radius*Math.cos(angle);
    const y = -radius*Math.sin(angle);
    c.innerHTML += `<line class='note-line' x2='${x}' y2='${y}' stroke='whitesmoke' stroke-width='0.2px'/>`;
    c.innerHTML += `<text x=${1.05*x} y=${1.05*y} fill='lightblue' font-size='0.2rem' text-anchor='middle' dominant-baseline='middle'>${noteNames[i]}</text>`;
  }
}

function drawSpiral() {
  const b = Math.LN2 / 2 / Math.PI;
  const centsPerStep = 25;
  const numOctaves = 8;
  const numSteps = numOctaves * 1200 / centsPerStep;
  const angleStep = 2*Math.PI * centsPerStep / 1200;
  const radiusStart = scale/400;
  c.innerHTML += `<path id='spiral' d='M 0 ${-radiusStart}' stroke='gray' stroke-width='0.2px' fill='none'/>`;
  const p = document.getElementById("spiral");

  for (let i=1; i < numSteps+1; i++) {
    const angle = i*angleStep + Math.PI/2;
    const radius = radiusStart * Math.exp(b*(angle-Math.PI/2));
    const x = radius*Math.cos(angle);
    const y = -radius*Math.sin(angle);
    p.setAttribute('d', p.getAttribute('d') + ` ${x} ${y}`);
  }
}

drawNoteLines();
drawSpiral();