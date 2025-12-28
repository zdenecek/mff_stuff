export interface Article {

  identifier: string

  title: string

}

export async function fetchArticles(): Promise<Article[]> {
  try {
    return await (await fetch("./api/v1/articles")).json()
  } catch (error) {
    console.error("Can't fetch articles.", error)
    return []
  }
}
