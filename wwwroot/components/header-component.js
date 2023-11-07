class HeaderComponent extends HTMLDivElement {
    constructor() {
        super();
        this.innerHTML = `
          <nav class="bg-gray-100 flex gap-5 p-8 border-b-[0.5px] border-gray-600/20 mb-4">
            <a href="index.html" class="text-lg hover:-translate-y-1 transition-all p-1 hover:text-gray-800">Home</a>
            <a href="foo.html" class="text-lg hover:-translate-y-1 transition-all p-1 hover:text-gray-800">Foo</a>
            <a href="upload.html" class="text-lg hover:-translate-y-1 transition-all p-1 hover:text-gray-800">Upload</a>
          </nav>
        `;
    }
}

customElements.define('header-component', HeaderComponent, { extends: 'div' });