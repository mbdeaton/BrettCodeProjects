# React Demos
Brett Deaton - Feb 2023

Some demos for the QuickBytes presentation for ERP engineers 2.16.23.
I'm assuming beginner's level knowledge about front-end web development,
like I had a year ago.


### Main Points
Major Benefits of React:

* **Virtual DOM**: keeps UI in sync with the state, and leads React syntax to be
  declarative ("just describe the components, React will rerender when needed,
  as if refreshing the page, but preserving DOM and state" -
  [paraphrase of Dan Abramov](https://twitter.com/dan_abramov/status/1623522710176374785)).
* **Components**: wed logic and view together into easy-to-understand, extendable,
  reusable modules.

How to:

* Create and use a React component
* Progressively include React in an existing Vanilla.JS app
* Build a React app ground-up with best practices

Some of the main categories of application development discussed when speaking
of separation of concerns. These are my own names, with examples from the demo
app below:

* Data: e.g. content like `"EA_STA_100"`
* View: e.g. HTML elements like `<div>`
* DOM Management: e.g. APIs like `createX()`
* Logic: e.g. data and event manipulations like `Array.map()` or `handleX()`


### Outline
Examine an "app" that's an editable table of test stations, a very simple
front-end-only mockup of something like might be used in a factory.

1. Audience experience with web development?
2. My journey to web dev from science computing. Browser: universal platform!
3. Demo some full-fledged React App
4. Let's simplify. The hierarchy of simple apps I'll demo: html->React.
5. html
6. Visualize the DOM.
6. html-js
7. html-js-react
8. html-js-react-cra


### Create-React-App Guide
```
> npx create-react-app my-app --template minimal    # minimal template is a *lot* simpler than the default CRA template
> npm start                                         # start local server and hot module reloading
> cd my-app/src                                     # add your code here
```

### References
* [Official React tutorial](https://reactjs.org/tutorial/tutorial.html)
* [KC Dodds Epic React course](https://epicreact.dev)
* [React resources](https://reactresources.com)
* [React patterns](https://reactpatterns.com)
* One suggested [file architecture from Create react-app](https://medium.com/geekculture/react-js-architecture-features-folder-structure-design-pattern-70b7b9103f22)
* [Live DOM Viewer](https://livedomviewer.com)
