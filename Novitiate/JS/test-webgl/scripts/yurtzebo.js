function main() {
    const canvas = document.querySelector('#glcanvas');
    const gl = canvas.getContext('webgl');
    // If we don't have a GL context, give up now.
    // Only continue if WebGL is available and working.
    if (!gl) {
        alert('Unable to initialize WebGL.'
              + ' Your browser or machine may not support it.');
        return;
    }
    // set clear color to black, fully opaque
    gl.clearColor(0.0, 0.0, 0.0, 1.0);
    // Clear the color buffer with specified clear color
    gl.clear(gl.COLOR_BUFFER_BIT);
}

main();
