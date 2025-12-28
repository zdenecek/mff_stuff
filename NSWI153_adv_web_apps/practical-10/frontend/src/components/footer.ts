
export function renderFooter() {
  // https://vitejs.dev/guide/env-and-mode.html#html-env-replacement
  document.querySelector<HTMLDivElement>('#footer')!.innerHTML = `
    Content by ${import.meta.env.VITE_INSTANCE_NAME}
`
}
