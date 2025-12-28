export interface Article {

  identifier: string

  title: string

}

export async function fetchArticles(): Promise<Article[]> {
  try {
    const res = await (await fetch("./api/v1/articles")).json()
    return res.articles;
  } catch (error) {
    console.error("Can't fetch articles.", error)
    return []
  }
}
