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
        // Prefer environment variables, fallback to .env file
        $env = fn($key, $default = null) => getenv($key) !== false ? getenv($key) : $default;
        $config = file_exists(__DIR__ . '/../../.env') ? parse_ini_file(__DIR__ . '/../../.env') : [];
        
        $paths = [__DIR__ . '/../Entity'];
        $isDevMode = true;

        $dbParams = [
            'driver' => $env('DATABASE_DRIVER', $config['DATABASE_DRIVER'] ?? 'mysqli'),
            'host' => $env('DATABASE_HOST', $config['DATABASE_HOST'] ?? 'localhost'),
            'user' => $env('DATABASE_USER', $config['DATABASE_USER'] ?? 'root'),
            'password' => $env('DATABASE_PASSWORD', $config['DATABASE_PASSWORD'] ?? ''),
            'dbname' => $env('DATABASE_NAME', $config['DATABASE_NAME'] ?? 'test'),
        ];

        $config = ORMSetup::createAnnotationMetadataConfiguration($paths, $isDevMode);
        
        // Enable PostgreSQL quote identifiers
        $config->setQuoteStrategy(new \Doctrine\ORM\Mapping\DefaultQuoteStrategy());
        
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