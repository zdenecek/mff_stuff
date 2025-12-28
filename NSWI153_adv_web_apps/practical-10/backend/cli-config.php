<?php

use Doctrine\ORM\Tools\Console\ConsoleRunner;
use App\Config\DoctrineConfig;

require __DIR__ . '/vendor/autoload.php';

$entityManager = DoctrineConfig::getEntityManager();

return ConsoleRunner::createHelperSet($entityManager); 