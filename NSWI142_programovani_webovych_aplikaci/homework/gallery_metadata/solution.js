function preprocessGalleryData(imgData) {
    let blocks = [];

    // create block for each image
    imgData.forEach((img) => {
        img.block = [img];
        blocks.push(img.block);
    });

    // merge blocks for similar imaages
    imgData.forEach((img) => {
        img.similar.forEach((simimg) => {
            // add all similar images to the same block
            if (img.block === simimg.block) return; // already merged


            img.block.push(simimg);
            simimg.block.forEach((other) => {
                if (!img.block.includes(other)) {
                    img.block.push(other);
                    other.block = img.block;
                }
            });

            // remove merged block
            blocks = blocks.filter((block) => block !== simimg.block);

            // update block for all images in merged block
            simimg.block = img.block;
        });
    });

    // sort block images by date
    blocks.forEach((block) => {
        block.sort((a, b) => a.created.getTime() - b.created.getTime() || imgData.indexOf(a) - imgData.indexOf(b));
    });

    // sort blocks by date
    blocks.sort((a, b) => a[0].created.getTime() - b[0].created.getTime() || imgData.indexOf(a) - imgData.indexOf(b));

    const res = [];

    // create blocks for non-similar images
    for (let i = 0; i < blocks.length; i++) {
        const first = i;
        let b = blocks[first];
        if (blocks[i].length === 1) {
            while (blocks.length >= i + 2 && blocks[i + 1].length == 1) {
                b = [...b, ...blocks[i + 1]];
                i++;
            }
        }
        res.push(b);
    }

    // remove metadata from images
    imgData.forEach((img) => (img.block = undefined));

    return res;
}

module.exports = { preprocessGalleryData };
