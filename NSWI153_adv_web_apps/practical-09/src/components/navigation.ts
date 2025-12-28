
export function renderNavigation() {
  document.querySelector<HTMLDivElement>('#navigation')!.innerHTML = `
  <nav>
    <a href="./index.html">Home</a>
    <a href="./list.html">Articles</a>
    <a href="./about.html">About</a>
  </nav>
`
}
