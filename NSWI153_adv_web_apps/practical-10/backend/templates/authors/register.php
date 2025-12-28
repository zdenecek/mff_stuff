<?php ob_start(); ?>

<h1>Register Author</h1>

<form method="post" action="<?= url('/register-author') ?>">
    <div class="mb-3">
        <label for="name" class="form-label">Name</label>
        <input type="text" class="form-control" id="name" name="name" required>
    </div>
    <button type="submit" class="btn btn-primary">Register</button>
</form>

<?php $content = ob_get_clean(); ?>
<?php require __DIR__ . '/../layout.php'; ?> 