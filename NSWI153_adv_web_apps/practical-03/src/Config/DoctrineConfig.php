<?php

namespace App\Config;

use App\Entity\Article;
use App\Entity\Author;
use App\Repository\ArticleRepository;
use App\Repository\AuthorRepository;
use DI\ContainerBuilder;
use Doctrine\DBAL\DriverManager;
use Doctrine\ORM\EntityManager;
use Doctrine\ORM\ORMSetup;
use Doctrine\ORM\Tools\Console\ConsoleRunner;
use Doctrine\ORM\Tools\Console\EntityManagerProvider\SingleManagerProvider;

class DoctrineConfig
{
    public static function getEntityManager(): EntityManager
    {
        $config = parse_ini_file(__DIR__ . '/../../.env');
        
        $paths = [__DIR__ . '/../Entity'];
        $isDevMode = true;

        $dbParams = [
            'driver' => 'mysqli',
            'host' => $config['DATABASE_HOST'] ?? 'localhost',
            'user' => $config['DATABASE_USER'] ?? 'root',
            'password' => $config['DATABASE_PASSWORD'] ?? '',
            'dbname' => $config['DATABASE_NAME'] ?? 'test',
        ];

        $config = ORMSetup::createAnnotationMetadataConfiguration($paths, $isDevMode);
        return EntityManager::create($dbParams, $config);
    }

    public static function configureContainer(ContainerBuilder $builder): void
    {
        $builder->addDefinitions([
            EntityManager::class => function () {
                return self::getEntityManager();
            },
            AuthorRepository::class => function (EntityManager $em) {
                return new AuthorRepository($em);
            },
            ArticleRepository::class => function (EntityManager $em) {
                return new ArticleRepository($em);
            }
        ]);
    }
} 