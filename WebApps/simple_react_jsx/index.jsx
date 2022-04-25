'use strict';

const rootElement = (
  <div id="contacts">
    <h1>Contacts</h1>
    <ul id="nav">
      <li><a href='mailto:brett.deaton@gmail.com'>Brett Deaton</a></li>
      <li><a href='mailto:darcideaton@gmail.com'>Darci Deaton</a></li>
    </ul>
  </div>
);

//const domNode = document.getElementById('root');
//ReactDOM.render(rootElement, domNode);

const domContainer = document.querySelector('#root');
const root = ReactDOM.createRoot(domContainer);
root.render(rootElement, domContainer);