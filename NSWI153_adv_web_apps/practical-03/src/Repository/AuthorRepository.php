<?php

namespace App\Repository;

use App\Entity\Author;
use Doctrine\ORM\EntityRepository;
use Doctrine\ORM\EntityManager;

class AuthorRepository extends EntityRepository
{
    public function __construct(EntityManager $em)
    {
        parent::__construct($em, $em->getClassMetadata(Author::class));
    }

    public function findByName(string $name): ?Author
    {
        return $this->findOneBy(['name' => $name]);
    }

    public function save(Author $author): void
    {
        $this->getEntityManager()->persist($author);
        $this->getEntityManager()->flush();
    }
} 