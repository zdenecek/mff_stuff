package cz.cuni.mff.d3s.nswi080.documents;

import com.hazelcast.client.HazelcastClient;
import com.hazelcast.client.config.ClientConfig;
import com.hazelcast.core.HazelcastInstance;
import com.hazelcast.map.IMap;
import com.hazelcast.core.IExecutorService;
import com.hazelcast.map.EntryProcessor;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;
import java.util.concurrent.ExecutionException;

public class Client {
    /**
     * Reader from the standard input.
     */
    private final BufferedReader in = new BufferedReader(new InputStreamReader(System.in));

    /**
     * Connection to the cluster.
     */
    private HazelcastInstance hazelcast;

    /**
     * The name of the user.
     */
    private final String userName;

    // map names
    private static final String DOCUMENTS_CACHE = "documents";
    private static final String SELECTED_DOCUMENTS = "selectedDocuments";
    private static final String VIEW_COUNTS = "viewCounts";
    private static final String COMMENTS = "comments";
    private static final String FAVORITES = "favorites";
    
    // Do not keep any other state here - all data should be in the cluster.

    /**
     * Creates a client for the specified user.
     *
     * @param userName username to identify the user
     */
    public Client(String userName) {
        this.userName = userName;
        // Connect to the Hazelcast cluster.
        ClientConfig config = new ClientConfig();
        hazelcast = HazelcastClient.newHazelcastClient(config);
    }

    /**
     * Disconnect from the Hazelcast cluster.
     */
    public void disconnect() {
        // Disconnect from the Hazelcast cluster
        hazelcast.shutdown();
    }

    /**
     * Gets the currently selected document name for the user.
     * 
     * @return the name of the selected document or null if no document is selected
     */
    private String getSelectedDocumentName() {
        IMap<String, String> selectedDocuments = hazelcast.getMap(SELECTED_DOCUMENTS);
        return selectedDocuments.get(userName);
    }

    /**
     * Sets the currently selected document name for the user.
     * 
     * @param documentName the name of the document to select
     */
    private void setSelectedDocument(String documentName) {
        IMap<String, String> selectedDocuments = hazelcast.getMap(SELECTED_DOCUMENTS);
        selectedDocuments.put(userName, documentName);
    }

    /**
     * Gets a document from the cache or generates it if not cached.
     * 
     * @param documentName the name of the document to get
     * @return the document
     */
    private Document getDocument(String documentName) {
        IMap<String, Document> documents = hazelcast.getMap(DOCUMENTS_CACHE);
        
        // check cache first
        Document document = documents.get(documentName);
        
        if (document == null) {
            // not in cache, generate it on server
            IExecutorService executorService = hazelcast.getExecutorService("document-generator");
            try {
                document = executorService.submit(new GenerateDocumentTask(documentName)).get();
                // store in cache
                documents.put(documentName, document);
            } catch (InterruptedException | ExecutionException e) {
                System.err.println("Error generating document: " + e.getMessage());
                // fallback to local generation if needed
                document = DocumentGenerator.generateDocument(documentName);
            }
        }
        
        return document;
    }

    /**
     * Increments the view count for a document.
     * 
     * @param documentName the name of the document to increment the view count for
     * @return the new view count
     */
    private long incrementViewCount(String documentName) {
        IMap<String, Long> viewCounts = hazelcast.getMap(VIEW_COUNTS);
        return viewCounts.executeOnKey(documentName, new IncrementViewCountProcessor());
    }

    /**
     * Reads a name of a document, selects it as the current document of the user, and shows
     * the document content.
     */
    private void showCommand() throws IOException {
        System.out.println("Enter document name:");
        String documentName = in.readLine();

        // save as selected document
        setSelectedDocument(documentName);
        
        // get document
        Document document = getDocument(documentName);
        
        // count view
        incrementViewCount(documentName);

        // show content
        System.out.println("The document is:");
        System.out.println(document.getContent());
    }

    /**
     * Shows the next document in the list of favorites of the user.
     * Selects the next document, so that running this command repeatedly
     * will cyclically show all favorite documents of the user.
     */
    private void nextFavoriteCommand() {
        IMap<String, List<String>> favorites = hazelcast.getMap(FAVORITES);
        List<String> favoriteList = favorites.getOrDefault(userName, new ArrayList<>());
        
        if (favoriteList.isEmpty()) {
            System.out.println("Your favorites list is empty.");
            return;
        }
        
        String currentDocument = getSelectedDocumentName();
        String nextDocument;
        
        if (currentDocument == null || !favoriteList.contains(currentDocument)) {
            // if no selection or not in favs, start with first
            nextDocument = favoriteList.get(0);
        } else {
            // find current and get next
            int currentIndex = favoriteList.indexOf(currentDocument);
            int nextIndex = (currentIndex + 1) % favoriteList.size();
            nextDocument = favoriteList.get(nextIndex);
        }
        
        // select next doc
        setSelectedDocument(nextDocument);
        
        // get and show document
        Document document = getDocument(nextDocument);
        
        // count view
        incrementViewCount(nextDocument);
        
        System.out.println("The document is:");
        System.out.println(document.getContent());
    }

    /**
     * Adds the current selected document name to the list of favorite documents of the user.
     * If the list already contains the document name, do nothing.
     */
    private void addFavoriteCommand() {
        String selectedDocumentName = getSelectedDocumentName();
        
        if (selectedDocumentName == null) {
            System.out.println("No document selected. Select a document first with 's' command.");
            return;
        }
        
        IMap<String, List<String>> favorites = hazelcast.getMap(FAVORITES);
        favorites.executeOnKey(userName, new AddToFavoritesProcessor(selectedDocumentName));
        
        System.out.printf("Added %s to favorites%n", selectedDocumentName);
    }

    /**
     * Removes the current selected document name from the list of favorite documents of the user.
     * If the list does not contain the document name, do nothing.
     */
    private void removeFavoriteCommand() {
        String selectedDocumentName = getSelectedDocumentName();
        
        if (selectedDocumentName == null) {
            System.out.println("No document selected. Select a document first with 's' command.");
            return;
        }
        
        IMap<String, List<String>> favorites = hazelcast.getMap(FAVORITES);
        favorites.executeOnKey(userName, new RemoveFromFavoritesProcessor(selectedDocumentName));
        
        System.out.printf("Removed %s from favorites%n", selectedDocumentName);
    }

    /**
     * Lists all favorite documents of the user.
     */
    private void listFavoritesCommand() {
        IMap<String, List<String>> favorites = hazelcast.getMap(FAVORITES);
        List<String> favoriteList = favorites.getOrDefault(userName, new ArrayList<>());
        
        System.out.println("Your list of favorite documents:");
        if (favoriteList.isEmpty()) {
            System.out.println("(empty)");
        } else {
            for (String favoriteDocumentName : favoriteList) {
                System.out.println(favoriteDocumentName);
            }
        }
    }

    /**
     * Shows the view count and comments of the current selected document.
     */
    private void infoCommand() {
        String selectedDocumentName = getSelectedDocumentName();
        
        if (selectedDocumentName == null) {
            System.out.println("No document selected. Select a document first with 's' command.");
            return;
        }
        
        // get view count
        IMap<String, Long> viewCounts = hazelcast.getMap(VIEW_COUNTS);
        long viewCount = viewCounts.getOrDefault(selectedDocumentName, 0L);
        
        // get comments
        IMap<String, List<String>> commentsMap = hazelcast.getMap(COMMENTS);
        List<String> comments = commentsMap.getOrDefault(selectedDocumentName, new ArrayList<>());
        
        // print
        System.out.printf("Info about %s:%n", selectedDocumentName);
        System.out.printf("Viewed %d times.%n", viewCount);
        System.out.printf("Comments (%d):%n", comments.size());
        for (String comment : comments) {
            System.out.println(comment);
        }
    }

    /**
     * Adds a comment to the current selected document.
     */
    private void commentCommand() throws IOException {
        String selectedDocumentName = getSelectedDocumentName();
        
        if (selectedDocumentName == null) {
            System.out.println("No document selected. Select a document first with 's' command.");
            return;
        }
        
        System.out.println("Enter comment text:");
        String commentText = in.readLine();
        
        String formattedComment = userName + ": " + commentText;
        
        // add comment
        IMap<String, List<String>> commentsMap = hazelcast.getMap(COMMENTS);
        commentsMap.executeOnKey(selectedDocumentName, new AddCommentProcessor(formattedComment));
        
        System.out.printf("Added a comment about %s.%n", selectedDocumentName);
    }

    /*
     * Runs the main interactive loop.
     */
    public void run() throws IOException {
        loop: while (true) {
            System.out.println("\nAvailable commands (type and press enter):");
            System.out.println(" s - select and show document");
            System.out.println(" i - show document view count and comments");
            System.out.println(" c - add comment");
            System.out.println(" a - add to favorites");
            System.out.println(" r - remove from favorites");
            System.out.println(" n - show next favorite");
            System.out.println(" l - list all favorites");
            System.out.println(" q - quit");
            String line = in.readLine();
            if (line.isEmpty()) {
                continue;
            }
            switch (line.charAt(0)) {
                case 'q': // Quit the application.
                    break loop;
                case 's': // Select and show a document.
                    showCommand();
                    break;
                case 'i': // Show view count and comments of the selected document.
                    infoCommand();
                    break;
                case 'c': // Add a comment to the selected document.
                    commentCommand();
                    break;
                case 'a': // Add the selected document to favorites.
                    addFavoriteCommand();
                    break;
                case 'r': // Remove the selected document from favorites.
                    removeFavoriteCommand();
                    break;
                case 'n': // Select and show the next document in the list of favorites.
                    nextFavoriteCommand();
                    break;
                case 'l': // Show the list of favorite documents.
                    listFavoritesCommand();
                    break;
                default:
                    break;
            }
        }
    }

    /*
     * Main method, creates a client instance and runs its loop.
     */
    public static void main(String[] args) throws IOException {
        if (args.length != 1) {
            System.err.println("Usage: java Client <userName>");
            return;
        }

        Client client = new Client(args[0]);
        try {
            client.run();
        } finally {
            client.disconnect();
        }
    }
}

// task to generate document on server
class GenerateDocumentTask implements Serializable, java.util.concurrent.Callable<Document> {
    private final String documentName;
    
    public GenerateDocumentTask(String documentName) {
        this.documentName = documentName;
    }
    
    @Override
    public Document call() {
        return DocumentGenerator.generateDocument(documentName);
    }
}

// processor for incrementing view count atomically
class IncrementViewCountProcessor implements EntryProcessor<String, Long, Long>, Serializable {
    @Override
    public Long process(Map.Entry<String, Long> entry) {
        Long viewCount = entry.getValue();
        if (viewCount == null) {
            viewCount = 0L;
        }
        viewCount++;
        entry.setValue(viewCount);
        return viewCount;
    }
}

// processor to add document to favorites
class AddToFavoritesProcessor implements EntryProcessor<String, List<String>, Void>, Serializable {
    private final String documentName;
    
    public AddToFavoritesProcessor(String documentName) {
        this.documentName = documentName;
    }
    
    @Override
    public Void process(Map.Entry<String, List<String>> entry) {
        List<String> favorites = entry.getValue();
        if (favorites == null) {
            favorites = new ArrayList<>();
        }
        if (!favorites.contains(documentName)) {
            favorites.add(documentName);
            entry.setValue(favorites);
        }
        return null;
    }
}

// processor to remove document from favorites
class RemoveFromFavoritesProcessor implements EntryProcessor<String, List<String>, Void>, Serializable {
    private final String documentName;
    
    public RemoveFromFavoritesProcessor(String documentName) {
        this.documentName = documentName;
    }
    
    @Override
    public Void process(Map.Entry<String, List<String>> entry) {
        List<String> favorites = entry.getValue();
        if (favorites != null) {
            favorites.remove(documentName);
            entry.setValue(favorites);
        }
        return null;
    }
}

// processor to add comment to document
class AddCommentProcessor implements EntryProcessor<String, List<String>, Void>, Serializable {
    private final String comment;
    
    public AddCommentProcessor(String comment) {
        this.comment = comment;
    }
    
    @Override
    public Void process(Map.Entry<String, List<String>> entry) {
        List<String> comments = entry.getValue();
        if (comments == null) {
            comments = new ArrayList<>();
        }
        comments.add(comment);
        entry.setValue(comments);
        return null;
    }
}
