let createElement = React.createElement
      
let rootElement =
  createElement('div', {},
    createElement('hi', {}, "Contacts"),
    createElement('ul', {id: 'nav'},
      createElement(
        'li',
        {},
        createElement(
          'a',
          { href: 'mailto:brett.deaton@gmail.com' },
          "Brett Deaton"
        ),
      ),
      createElement(
        'li',
        {},
        createElement(
          'a',
          { href: 'mailto:darcideaton@gmail.com' },
          "Darci Deaton"
        )
      )
    )
  )
let domNode = document.getElementById('root')
ReactDOM.render(rootElement, domNode)