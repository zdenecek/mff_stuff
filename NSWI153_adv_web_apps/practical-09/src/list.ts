import './style.scss'
import {renderNavigation} from './components/navigation'
import {renderFooter} from './components/footer'
import {Article, fetchArticles} from './services/articles-service'

(async function initialize() {
  renderNavigation()
  renderFooter()
  //
  const articles = await fetchArticles()
  const element = document.querySelector<HTMLDivElement>('#articles')!
  for (const article of articles) {
    renderArticle(element, article);
  }
  console.log(articles)
})()

function renderArticle(parent: HTMLDivElement, article : Article)  {
  const listItem = document.createElement('li')
  listItem.classList.add("item")
  listItem.innerHTML =
  `<a href="./api/v1/article-detail/${article.identifier}">
    ${article.title}
  </h2>`
  parent.appendChild(listItem);
}
