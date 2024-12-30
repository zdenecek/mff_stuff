-- name: GetTwytts :many
SELECT * FROM twytts;

-- name: InsertTwytt :one
INSERT INTO twytts (
	name,
	twytt
)
VALUES (
	$1,
	$2
)
RETURNING *;

-- name: DeleteTwytt :one
DELETE FROM twytts 
WHERE id = $1
RETURNING *;

-- name: UpdateTwytt :one
UPDATE twytts SET name=$2, twytt=$3 
WHERE id = $1
RETURNING *;