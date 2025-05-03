package cz.cuni.mff.d3s.nswi080.documents;

import java.io.Serializable;

/**
 * Represents a document that can be displayed to the user.
 */
public class Document implements Serializable {
    private final String content;

    public Document(String content) {
        this.content = content;
    }

    public String getContent() {
        return content;
    }
}
