<?php ob_start(); ?>

<h1>Articles</h1>

<div class="list-group">
    <?php foreach ($articles as $article): ?>
        <div class="list-group-item">
            <h5 class="mb-1"><?= htmlspecialchars($article->getTitle()) ?></h5>
            <p class="mb-1">By: <?= htmlspecialchars($article->getAuthor()->getName()) ?></p>
            <p class="mb-1"><?= htmlspecialchars($article->getContent()) ?></p>
            <small>
                <a href="<?= url('/api/v1/article-detail/' . $article->getId()) ?>" class="text-muted">View API Response</a>
            </small>
        </div>
    <?php endforeach; ?>
</div>

<?php $content = ob_get_clean(); ?>
<?php require __DIR__ . '/../layout.php'; ?> 