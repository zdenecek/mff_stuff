package cz.cuni.mff.d3s.nswi080.documents;

public class DocumentGenerator {
    /**
     * Simulates a long-running operation that generates a document.
     *
     * @param documentName the name of the document
     * @return the generated document
     */
    static Document generateDocument(String documentName) {
        try {
            System.out.println("Generating document " + documentName);
            Thread.sleep(3000);
        } catch (InterruptedException ie) {
            // Ignored.
        }
        return new Document("==================================\n" +
                "This is the document named " + documentName + "\n" +
                "==================================");
    }
}
