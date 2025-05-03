function rectanglesOverlap(r1, r2) {
    return (
        r1.left < r2.left + r2.width &&
        r1.top < r2.top + r2.height &&
        r2.left < r1.left + r1.width &&
        r2.top < r1.top + r1.height
    );
}

function processRect(set, f, r) {
	if (!rectanglesOverlap(f, r)) {
		set.add(f);
		return;
	}
	
	set.delete(f);

	if (f.top < r.top) {
		set.add({ top: f.top, left: f.left, width: f.width, height: r.top - f.top });
	}
	if (f.left < r.left) {
		set.add({ top: f.top, left: f.left, width: r.left - f.left, height: f.height });
	}
	if (f.left + f.width > r.left + r.width) {
		set.add({ top: f.top, left: r.left + r.width, height: f.height, width: f.left + f.width - (r.left + r.width) });
	}
	if ((r.top + r.height) <  (f.top + f.height)) {
		set.add({ top: (r.top + r.height), left: f.left, height:  (f.top + f.height) - (r.top + r.height), width: f.width });
	}
}

function maxFreeRect(width, height, rects) {
    
    let free = new Set([{ left: 0, top: 0, width: width, height: height }]);

    rects.forEach((r) => {
        let set = new Set();
        free.forEach((f) => {
            processRect(set, f, r);
        });
        free = set;
    });

    if(free.size === 0)  return {
        left: 0,
        top: 0,
        width: 0,
        height: 0,
    };

	free.forEach(f => f.area = f.width * f.height);
	return Array.from(free).reduce((a, b) => a.area > b.area ? a : b);
}

module.exports = { maxFreeRect };
