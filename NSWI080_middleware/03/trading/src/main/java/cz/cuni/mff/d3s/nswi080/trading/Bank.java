package cz.cuni.mff.d3s.nswi080.trading;

import java.io.IOException;
import java.util.HashMap;
import java.util.Map;

import javax.jms.Connection;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MapMessage;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.MessageListener;
import javax.jms.MessageProducer;
import javax.jms.Queue;
import javax.jms.Session;
import javax.jms.TextMessage;

import org.apache.activemq.artemis.jms.client.ActiveMQConnectionFactory;

public class Bank implements MessageListener {
    /**
     * Text message command to open a new account.
     */
    public static final String NEW_ACCOUNT_MSG = "NEW_ACCOUNT";
     /**
     * Text message command to open check account balance.
     */
    public static final String CHECK_BALANCE_MSG = "CHECK_BALANCE";
    

    /**
     * MapMessage key for order type.
     */
    public static final String ORDER_TYPE_KEY = "orderType";
    /**
     * MapMessage key for balance.
     */
    public static final String BALANCE_KEY = "balance";
    /**
     * MapMessage key for account number.
     */
    public static final String ACCOUNT_NUMBER_KEY = "accountNumber";

    /**
     * Order type "send money".
     */
    public static final int ORDER_TYPE_SEND = 1;

    /**
     * MapMessage key for receiver's account number.
     */
    public static final String ORDER_RECEIVER_ACC_KEY = "receiverAccount";

    /**
     * MapMessage key for amount of money transferred.
     */
    public static final String AMOUNT_KEY = "amount";

    /**
     * The name of the queue for sending messages to the bank.
     */
    public static final String BANK_QUEUE = "BankQueue";

    /**
     * MapMessage key for report type.
     */
    public static final String REPORT_TYPE_KEY = "reportType";

    /**
     * Report type "received money".
     */
    public static final int REPORT_TYPE_RECEIVED = 1;

    /**
     * Report type "Insufficient balance".
     */
    public static final int REPORT_TYPE_INSUFFICIENT_BALANCE = 2;

    /**
     * MapMessage key for sender's account.
     */
    public static final String REPORT_SENDER_ACC_KEY = "senderAccount";
    static String ORDER_METADATA_KEY = "metadata";

    /**
     * Connection to the broker.
     */
    private final Connection conn;

    /**
     * Session for asynchronous event messages.
     */
    private Session bankSession;

    /**
     * Sender of (reply) messages, not bound to any destination.
     */
    private MessageProducer bankSender;

    /**
     * The receiver of event messages.
     */
    private MessageConsumer bankReceiver;

    /**
     * The queue of incoming messages.
     */
    private Queue toBankQueue;

    /**
     * The last assigned account number.
     */
    private int lastAccount  = 1000000;

    /**
     * Maps client names to client account numbers.
     */
    private final Map<String, Integer> clientAccounts = new HashMap<>();

    /**
     * Maps client account numbers to client names.
     */
    private final Map<Integer, String> accountsClients = new HashMap<>();


    /**
     * Maps client names to client report destinations.
     */
    private final Map<String, Destination> clientDestinations = new HashMap<>();

    // TODO: store and check account balance
    private final Map<Integer, Integer> clientAccountBalances = new HashMap<>();

    private static final int SEED_MONEY = 1000; // we provide soem seed money so that any trades can be made
    
    // in the current implementation, a transfer always succeeds
    // (1) check if the client has enough money
    // (2) if not, send a message that the transfer failed instead
    // (3) if yes, send the messages that the transfer succeeded and decrease the balance

    // TODO: add a command to get current account balance

    /*
     * Constructor, stores the broker connection and initializes maps.
     */
    private Bank(Connection conn) {
        this.conn = conn;
    }

    /*
     * Initializes messaging structures, starts listening for messages
     */
    private void init() throws JMSException {
        // create a non-transacted, auto acknowledged session
        bankSession = conn.createSession(false, Session.AUTO_ACKNOWLEDGE);

        // create queue for incoming messages
        toBankQueue = bankSession.createQueue(BANK_QUEUE);

        // create consumer of incoming messages
        bankReceiver = bankSession.createConsumer(toBankQueue);

        // receive messages asynchronously, using this object's onMessage()
        bankReceiver.setMessageListener(this);

        // create producer of messages, not bound to any destination
        bankSender = bankSession.createProducer(null);

        // start processing incoming messages
        conn.start();
    }

    /*
     * Handles text messages - in our case it's only the message requesting new account.
     */
    private void processTextMessage(TextMessage txtMsg) throws JMSException {
        // get the destination that client specified for replies
        // we will use it to send reply and also store it for transfer report messages
        Destination replyDest = txtMsg.getJMSReplyTo();
        if (null == txtMsg.getText()) {
            System.out.println("Received unknown text message: " + txtMsg.getText());
            System.out.println("Full message info:\n" + txtMsg);
        } else 
        switch (txtMsg.getText()) {
            case NEW_ACCOUNT_MSG ->                 {
                    // get the client's name stored as a property
                    String clientName = txtMsg.getStringProperty(Client.CLIENT_NAME_PROPERTY);
                    int accountNumber = processAccountCreation(  clientName ,  replyDest);
                    // create reply TextMessage with the account number
                    TextMessage reply = bankSession.createTextMessage(String.valueOf(accountNumber));
                    // send the reply to the provided reply destination
                    bankSender.send(replyDest, reply);
                }
            // a command to get current account balance
            case CHECK_BALANCE_MSG ->                 {
                    int accountNumber = txtMsg.getIntProperty(ACCOUNT_NUMBER_KEY);
                    TextMessage replyMessage = bankSession.createTextMessage();
                    if (accountsClients.containsKey(accountNumber)) {
                        replyMessage.setIntProperty(BALANCE_KEY, clientAccountBalances.get(accountNumber));
                    }
                    else {
                        replyMessage.setText("Account does not exist");
                    }       bankSender.send(replyDest, replyMessage);
                }
            default -> {
                System.out.println("Received unknown text message: " + txtMsg.getText());
                System.out.println("Full message info:\n" + txtMsg);
            }
        }
    }

    private int processAccountCreation( String clientName , Destination replyDest) {

            // store client's reply destination for future transfer reports
            clientDestinations.put(clientName, replyDest);

            int accountNumber;
            // either assign new account number or return already known number
            if (clientAccounts.get(clientName) != null) {
                accountNumber = clientAccounts.get(clientName);
            } else {
                accountNumber = lastAccount++;
                // also store the newly assigned number
                clientAccounts.put(clientName, accountNumber);
                accountsClients.put(accountNumber, clientName);
                clientAccountBalances.put(accountNumber, 0);
            }
            if (SEED_MONEY > 0)
                clientAccountBalances.put(accountNumber, SEED_MONEY);


            System.out.println("Connected client " + clientName + " with account " + accountNumber);

            return accountNumber;
    }

    /*
     * Handles map messages - in our case it's only the message ordering money transfer to a receiver account.
     */
    private void processMapMessage(MapMessage mapMsg) throws JMSException {
        // get the order type number
        int order = mapMsg.getInt(ORDER_TYPE_KEY);

        // process order to transfer money
        if (order == ORDER_TYPE_SEND) {
            // get client's name
            String clientName = mapMsg.getStringProperty(Client.CLIENT_NAME_PROPERTY);

            // find client's account number
            int clientAccount = clientAccounts.get(clientName);

            // get receiver account number
            int destAccount = mapMsg.getInt(ORDER_RECEIVER_ACC_KEY);

            // find receiving client's name
            String destName = accountsClients.get(destAccount);

           
            // get amount of money being transferred
            int amount = mapMsg.getInt(AMOUNT_KEY);



              // create report message 
            MapMessage reportMsg = bankSession.createMapMessage();

            // set sender's account number
            reportMsg.setInt(REPORT_SENDER_ACC_KEY, clientAccount);

            // set money of amount transfered
            reportMsg.setInt(AMOUNT_KEY, amount);
            
             // find receiving client's report message destination
            Destination dest;

            synchronized(clientAccountBalances) {
                var balance = clientAccountBalances.get(clientAccount);
                // (1) check if the client has enough money
                if (balance <= amount) {
                    // (2) if not, send a message that the transfer failed instead
                    System.out.println("Insufficient balance $" + balance + " in account " + clientAccount + " for transfer of $"  + amount + " to account " + destAccount);
                    reportMsg.setInt(REPORT_TYPE_KEY, REPORT_TYPE_INSUFFICIENT_BALANCE);
                    reportMsg.setInt(ORDER_RECEIVER_ACC_KEY, destAccount);
                    reportMsg.setStringProperty(ORDER_METADATA_KEY, mapMsg.getStringProperty(ORDER_METADATA_KEY));
                    dest = clientDestinations.get(clientName);  

                    
                } else {
                    // (3) if yes,
                     System.out.println("Transferring $" + amount + " from account " + clientAccount + " to account " + destAccount);
                    // send the messages that the transfer succeeded 
                    dest = clientDestinations.get(destName);
                    // and decrease the balance
                    clientAccountBalances.put(clientAccount, balance - amount);
                    // set report type to "you received money"
                    reportMsg.setInt(REPORT_TYPE_KEY, REPORT_TYPE_RECEIVED);    
                }
            }
            // send report to receiver client's destination
            bankSender.send(dest, reportMsg);
        } else {
            System.out.println("Received unknown MapMessage:\n" + mapMsg);
        }
    }

    /*
     * Reacts to an asynchronously received message.
     */
    @Override
    public void onMessage(Message msg) {
        // distinguish type of message and call appropriate handler
        try {
            if (msg instanceof TextMessage) {
                processTextMessage((TextMessage) msg);
            } else if (msg instanceof MapMessage) {
                processMapMessage((MapMessage) msg);
            } else {
                System.out.println("Received unknown message:\n: " + msg);
            }
        } catch (JMSException e) {
            throw new RuntimeException(e);
        }
    }

    /*
     * Main method, creates a connection to the broker and a {@link Bank} instance.
     */
    public static void main(String[] args) throws JMSException, IOException {
        // create connection to the broker.

        try (ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory("tcp://localhost:61616");
             Connection connection = connectionFactory.createConnection()) {
            // create a bank instance
            Bank bank = new Bank(connection);
            // initialize bank's messaging
            bank.init();

            // bank now listens to asynchronous messages on another thread
            // wait for user before quit
            System.out.println("Bank running. Press enter to quit");

            System.in.read();

            System.out.println("Stopping...");
        }
    }
}
