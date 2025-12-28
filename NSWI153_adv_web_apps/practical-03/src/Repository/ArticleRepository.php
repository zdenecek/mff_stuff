<?php

namespace App\Repository;

use App\Entity\Article;
use Doctrine\ORM\EntityRepository;
use Doctrine\ORM\EntityManager;

class ArticleRepository extends EntityRepository
{
    public function __construct(EntityManager $em)
    {
        parent::__construct($em, $em->getClassMetadata(Article::class));
    }

    public function findByAuthorName(?string $authorName = null): array
    {
        $qb = $this->createQueryBuilder('a')
            ->join('a.author', 'author')
            ->addSelect('author');

        if ($authorName !== null) {
            $qb->where('author.name = :authorName')
               ->setParameter('authorName', $authorName);
        }

        return $qb->getQuery()->getResult();
    }

    public function save(Article $article): void
    {
        $this->getEntityManager()->persist($article);
        $this->getEntityManager()->flush();
    }
} 