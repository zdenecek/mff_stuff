<?php

namespace App\Controller;

use App\Entity\Article;
use App\Entity\Author;
use App\Repository\ArticleRepository;
use App\Repository\AuthorRepository;
use Psr\Http\Message\ResponseInterface as Response;
use Psr\Http\Message\ServerRequestInterface as Request;
use Slim\Views\PhpRenderer;

class ArticleController
{
    private PhpRenderer $view;
    private ArticleRepository $articleRepository;
    private AuthorRepository $authorRepository;

    public function __construct(
        PhpRenderer $view,
        ArticleRepository $articleRepository,
        AuthorRepository $authorRepository
    ) {
        $this->view = $view;
        $this->articleRepository = $articleRepository;
        $this->authorRepository = $authorRepository;
    }

    public function list(Request $request, Response $response): Response
    {
        $articles = $this->articleRepository->findByAuthorName();
        return $this->view->render($response, 'articles/list.php', ['articles' => $articles]);
    }

    public function createForm(Request $request, Response $response): Response
    {
        $authors = $this->authorRepository->findAll();
        return $this->view->render($response, 'articles/create.php', ['authors' => $authors]);
    }

    public function create(Request $request, Response $response): Response
    {
        $data = $request->getParsedBody();
        $title = $data['title'] ?? '';
        $content = $data['content'] ?? '';
        $authorId = $data['author_id'] ?? null;

        if (empty($title) || empty($content) || empty($authorId)) {
            $authors = $this->authorRepository->findAll();
            return $this->view->render($response, 'articles/create.php', [
                'authors' => $authors,
                'error' => 'All fields are required'
            ]);
        }

        $author = $this->authorRepository->find($authorId);
        if (!$author) {
            $authors = $this->authorRepository->findAll();
            return $this->view->render($response, 'articles/create.php', [
                'authors' => $authors,
                'error' => 'Invalid author'
            ]);
        }

        $article = new Article();
        $article->setTitle($title);
        $article->setContent($content);
        $article->setAuthor($author);
        $this->articleRepository->save($article);

        return $response->withHeader('Location', url('/articles'))->withStatus(302);
    }

    public function detail(Request $request, Response $response, array $args): Response
    {
        $article = $this->articleRepository->find($args['id']);
        if (!$article) {
            return $response->withStatus(404);
        }

        $response->getBody()->write(json_encode([
            'identifier' => (string)$article->getId(),
            'title' => $article->getTitle(),
            'content' => $article->getContent(),
            'author' => [
                'identifier' => (string)$article->getAuthor()->getId(),
                'name' => $article->getAuthor()->getName()
            ]
        ]));

        return $response->withHeader('Content-Type', 'application/json');
    }

    public function listApi(Request $request, Response $response): Response
    {
        $authorName = $request->getQueryParams()['author'] ?? null;
        $articles = $this->articleRepository->findByAuthorName($authorName);

        $data = [
            'articles' => array_map(function (Article $article) {
                return [
                    'identifier' => (string)$article->getId(),
                    'title' => $article->getTitle()                  
                ];
            }, $articles)
        ];

        $response->getBody()->write(json_encode($data));
        return $response->withHeader('Content-Type', 'application/json');
    }

    public function createApi(Request $request, Response $response): Response
    {
        $data = json_decode($request->getBody()->getContents(), true);
        $title = $data['title'] ?? '';
        $content = $data['content'] ?? '';
        $authorName = $data['author'] ?? '';

        if (empty($title) || empty($content) || empty($authorName)) {
            return $response->withStatus(400);
        }

        $author = $this->authorRepository->findByName($authorName);
        if (!$author) {
            $author = new Author();
            $author->setName($authorName);
            $this->authorRepository->save($author);
        }

        $article = new Article();
        $article->setTitle($title);
        $article->setContent($content);
        $article->setAuthor($author);
        $this->articleRepository->save($article);

        $response->getBody()->write(json_encode([
            'identifier' => (string)$article->getId(),
            // 'title' => $article->getTitle(),
            // 'content' => $article->getContent(),
            // 'author' => [
            //     'identifier' => (string)$article->getAuthor()->getId(),
            //     'name' => $article->getAuthor()->getName()
            // ]
        ]));

        return $response->withHeader('Content-Type', 'application/json');
    }
} 