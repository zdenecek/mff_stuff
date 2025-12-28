<?php ob_start(); ?>

<h1>Authors</h1>

<div class="list-group">
    <?php foreach ($authors as $author): ?>
        <div class="list-group-item">
            <h5 class="mb-1"><?= htmlspecialchars($author->getName()) ?></h5>
            <p class="mb-1">Articles:</p>
            <ul class="list-unstyled ms-3">
                <?php foreach ($author->getArticles() as $article): ?>
                    <li><?= htmlspecialchars($article->getTitle()) ?></li>
                <?php endforeach; ?>
            </ul>
        </div>
    <?php endforeach; ?>
</div>

<?php $content = ob_get_clean(); ?>
<?php require __DIR__ . '/../layout.php'; ?> 