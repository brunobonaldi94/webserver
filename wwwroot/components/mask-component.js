class MaskComponent extends HTMLElement {
  constructor() {
    super();
    this.onMutation = this.onMutation.bind(this);
    this.className = "lg:[clip-path:polygon(0_0,100%_0,100%_70vh,_0_100%)] md:[clip-path:polygon(0_0,100%_0,100%_75vh,_0_100%)] lg:h-[85vh] md:h-[80vh] bg-gray-100 space-y-2 flex flex-col"
    // this.innerHTML = `
    //   <main  class="[clip-path:polygon(0_0,100%_0,100%_65vh,_0_100%)] h-[85vh] bg-gray-100 space-y-2 flex flex-col">
    //   </main>
    // `
  }
  connectedCallback() {
    // Set up observer
    this.observer = new MutationObserver(this.onMutation);

    // Watch the Light DOM for child node changes
    this.observer.observe(this, {
      childList: true
    });
  }
  disconnectedCallback() {
    // remove observer if element is no longer connected to DOM
    this.observer.disconnect();
  }
  onMutation(mutations) {
    const added = [];

    // A `mutation` is passed for each new node
    for (const mutation of mutations) {
      // Could test for `mutation.type` here, but since we only have
      // set up one observer type it will always be `childList`
      added.push(...mutation.addedNodes);
    }
    
    console.log({
      // filter out non element nodes (TextNodes etc.)
      added: added.filter(el => el.nodeType === Node.ELEMENT_NODE),
    });
  }
}
function addCustomElement(){
  customElements.define('mask-component', MaskComponent);
}

addCustomElement();