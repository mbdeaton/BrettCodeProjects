let c = document.getElementById("spiral-canvas");
let ctx = c.getContext("2d");
let scale = c.clientWidth;
const radius = 0.9*scale/2;

function drawNoteLines() {
  const noteNames = ["C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"];
  const angleStep = 2*Math.PI / 12;

  for (let i = 0; i < noteNames.length; i++) {
    const angle = i*angleStep + Math.PI/2;
    const x = -radius*Math.cos(angle)+scale/2;
    const y = -radius*Math.sin(angle)+scale/2;
    ctx.moveTo(scale/2, scale/2);
    ctx.lineTo(x, y);
    ctx.stroke();
    ctx.fillText(noteNames[i], x, y);
  }
}

function drawSpiral() {
  const b = Math.LN2 / 2 / Math.PI;
  const centsPerStep = 50;
  const numOctaves = 5;
  const numSteps = numOctaves * 1200 / centsPerStep;
  const angleStep = 2*Math.PI * centsPerStep / 1200;
  const radiusStart = 10;
  ctx.moveTo(scale/2, scale/2)

  for (let i = 0; i < numSteps; i++) {
    const angle = i*angleStep + Math.PI/2;
    const radius = radiusStart * Math.exp(b*(angle-Math.PI/2));
    const x = -radius*Math.cos(angle)+scale/2;
    const y = -radius*Math.sin(angle)+scale/2;
    ctx.lineTo(x, y);
    ctx.stroke();
  }
}

drawNoteLines();
drawSpiral();