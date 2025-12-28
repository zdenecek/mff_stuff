<?php ob_start(); ?>

<h1>Create Article</h1>

<form method="post" action="<?= url('/create-article') ?>">
    <div class="mb-3">
        <label for="title" class="form-label">Title</label>
        <input type="text" class="form-control" id="title" name="title" required>
    </div>
    <div class="mb-3">
        <label for="content" class="form-label">Content</label>
        <textarea class="form-control" id="content" name="content" rows="5" required></textarea>
    </div>
    <div class="mb-3">
        <label for="author_id" class="form-label">Author</label>
        <select class="form-select" id="author_id" name="author_id" required>
            <option value="">Select an author</option>
            <?php foreach ($authors as $author): ?>
                <option value="<?= $author->getId() ?>"><?= htmlspecialchars($author->getName()) ?></option>
            <?php endforeach; ?>
        </select>
    </div>
    <button type="submit" class="btn btn-primary">Create Article</button>
</form>

<?php $content = ob_get_clean(); ?>
<?php require __DIR__ . '/../layout.php'; ?> 