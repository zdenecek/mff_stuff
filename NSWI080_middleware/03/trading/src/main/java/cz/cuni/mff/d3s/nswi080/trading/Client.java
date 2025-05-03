package cz.cuni.mff.d3s.nswi080.trading;

import java.io.IOException;
import java.io.InputStreamReader;
import java.io.LineNumberReader;
import java.io.Serializable;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Random;
import java.util.stream.Collectors;

import javax.jms.Connection;
import javax.jms.Destination;
import javax.jms.JMSException;
import javax.jms.MapMessage;
import javax.jms.Message;
import javax.jms.MessageConsumer;
import javax.jms.MessageListener;
import javax.jms.MessageProducer;
import javax.jms.ObjectMessage;
import javax.jms.Queue;
import javax.jms.Session;
import javax.jms.TextMessage;
import javax.jms.Topic;

import org.apache.activemq.artemis.jms.client.ActiveMQConnectionFactory;

public class Client {
    /**
     * The name of the property specifying client's name.
     */
    public static final String CLIENT_NAME_PROPERTY = "clientName";

    /**
     * The name of the topic for publishing offers.
     */
    public static final String OFFER_TOPIC = "Offers";

    public static final String CLIENT_MESSAGE_TYPE_KEY = "MessageType";
    public static final String CLIENT_MESSAGE_TYPE_GOODS_LISTING = "GoodsListing";
    public static final String CLIENT_MESSAGE_TYPE_OFFER = "Offer";
    public static final String CLIENT_MESSAGE_TYPE_OFFER_CANCELLATION = "OfferCancellation";
    public static final String CLIENT_ACCOUNT_NUMBER_KEY = "AccountNumber";
    public static final String CLIENT_GOODS_NAME_KEY = "GoodsName";
    public static final String CLIENT_OFFER_ACCEPTED_KEY = "IsOfferAccepted";
    public static final String CLIENT_PRICE_KEY = "Price";
    public static final String CLIENT_SALE_CONFIRMED = "IsConfirmed";
    public static final String CLIENT_MESSAGE_TYPE_REQUEST_OFFERS = "RequestOffers";
    /**
     * Client's unique name.
     */
    private final String clientName;

    /**
     * Client's account number.
     */
    private int accountNumber;

    /**
     * Offered goods, mapped by name.
     */
    private final Map<String, Goods> offeredGoods = new HashMap<>();

    /**
     * Available goods, mapped by seller's name.
     */
    private final Map<String, List<Goods>> availableGoods = new HashMap<>();

    /**
     * Reserved goods, mapped by name of the goods.
     */
    private final Map<String, Goods> reservedGoods = new HashMap<>();

    /**
     * Buyer's names, mapped by their account numbers.
     */
    private final Map<Integer, String> reserverAccounts = new HashMap<>();

    /**
     * Buyer's reply destinations, mapped by their names.
     */
    private final Map<String, Destination> reserverDestinations= new HashMap<>();

    /**
     * Connection to the broker.
     */
    private final Connection conn;

    /**
     * Session for user-initiated synchronous messages.
     */
    private Session clientSession;

    /**
     * Session for listening and reacting to asynchronous messages.
     */
    private Session eventSession;

    /**
     * Sender for the clientSession.
     */
    private MessageProducer clientSender;

    /**
     * Sender for the eventSession.
     */
    private MessageProducer eventSender;

    /**
     * Receiver of synchronous replies.
     */
    private MessageConsumer replyReceiver;

    /**
     * Topic to send and receiver offers.
     */
    private Topic offerTopic;

    /**
     * Queue for sending messages to the bank.
     */
    private Queue toBankQueue;

    /**
     * Queue for receiving synchronous replies.
     */
    private Queue replyQueue;

    /**
     * Line reader from stdin.
     */
    private final LineNumberReader in = new LineNumberReader(new InputStreamReader(System.in));


    /*
     * Client constructor. Initializes the maps.
     */
    private Client(String clientName, Connection conn) {
        this.clientName = clientName;
        this.conn = conn;
        generateGoods();
    }

    /*
     * Generates goods items.
     */
    private void generateGoods() {
        Random rnd = new Random();
        for (int i = 0; i < 10; ++i) {
            StringBuilder name = new StringBuilder();

            for (int j = 0; j < 4; ++j) {
                char c = (char) ('A' + rnd.nextInt('Z' - 'A'));
                name.append(c);
            }

            offeredGoods.put(name.toString(), new Goods(name.toString(), rnd.nextInt(10000)));
        }
    }

    /*
     * Sets up all JMS entities, gets bank account, publishes the first goods offer.
     */
    private void connect() throws JMSException {
        // create two sessions - one for synchronous and one for asynchronous processing
        clientSession = conn.createSession(false, Session.AUTO_ACKNOWLEDGE);
        eventSession = conn.createSession(false, Session.AUTO_ACKNOWLEDGE);

        // create (unbound) senders for the sessions
        clientSender = clientSession.createProducer(null);
        eventSender = eventSession.createProducer(null);

        // create queue for sending messages to bank
        toBankQueue = clientSession.createQueue(Bank.BANK_QUEUE);
        // create a temporary queue for receiving messages from bank
        Queue fromBankQueue = eventSession.createTemporaryQueue();

        // temporary receiver for the first reply from the bank
        // note that although the receiver is created within a different session
        // than the queue, it is OK since the queue is used only within the
        // client session for the moment
        MessageConsumer tmpBankReceiver = clientSession.createConsumer(fromBankQueue);

        // start processing messages
        conn.start();

        // request a bank account number
        Message msg = eventSession.createTextMessage(Bank.NEW_ACCOUNT_MSG);
        msg.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        // set ReplyTo that Bank will use to send me reply and later transfer reports
        msg.setJMSReplyTo(fromBankQueue);
        clientSender.send(toBankQueue, msg);

        // get reply from bank and store the account number
        TextMessage reply = (TextMessage) tmpBankReceiver.receive();
        accountNumber = Integer.parseInt(reply.getText());
        System.out.println("Account number: " + accountNumber);

        // close the temporary receiver
        tmpBankReceiver.close();

        // temporarily stop processing messages to finish initialization
        conn.stop();

        /* Processing bank reports */

        // create consumer of bank reports (from the fromBankQueue) on the event session
        MessageConsumer bankReceiver = eventSession.createConsumer(fromBankQueue);

        // set asynchronous listener for reports, using anonymous MessageListener
        // which just calls our designated method in its onMessage method
        bankReceiver.setMessageListener(new MessageListener() {
            @Override
            public void onMessage(Message msg) {
                try {
                    processBankReport(msg);
                } catch (JMSException e) {
                    throw new RuntimeException(e);
                }
            }
        });

        // TODO finish the initialization

        /* Step 1: Processing offers */

        // create a topic both for publishing and receiving offers
        // hint: Sessions have a createTopic() method
		offerTopic = eventSession.createTopic(OFFER_TOPIC);

        // create a consumer of offers from the topic using the event session
		replyReceiver = eventSession.createConsumer(offerTopic);

        // set asynchronous listener for offers (see above how it can be done)
        // which should call processOffer()
        replyReceiver.setMessageListener((Message m) -> {
            try {
                processOffer(m);               
            } catch (JMSException e) {
                e.printStackTrace();
                System.out.println("Error in receiving reply");
            }
        });

        /* Step 2: Processing sale requests */

        // create a queue for receiving sale requests (hint: Session has createQueue() method)
        // note that Session's createTemporaryQueue() is not usable here, the queue must have a name
        // that others will be able to determine from clientName (such as clientName + "SaleQueue")
		Queue saleRequests = eventSession.createQueue(getClientSaleQueName(clientName));

        // create consumer of sale requests on the event session
		MessageConsumer saleRequestConsumer = eventSession.createConsumer(saleRequests);

        // set asynchronous listener for sale requests (see above how it can be done)
        // which should call processSale()
        saleRequestConsumer.setMessageListener((Message m) -> {
            try {
                processSale(m);
            } catch (JMSException e) {
                e.printStackTrace();
                System.out.println("Error processing sale");
            }
        });

        // end TODO

        // create temporary queue for synchronous replies
        replyQueue = clientSession.createTemporaryQueue();

        // create synchronous receiver of the replies
        replyReceiver = clientSession.createConsumer(replyQueue);

        // restart message processing
        conn.start();

        // send list of offered goods
        publishGoodsList(clientSender, clientSession);

        // Request current offers from all clients
        TextMessage requestOffers = clientSession.createTextMessage(CLIENT_MESSAGE_TYPE_REQUEST_OFFERS);
        requestOffers.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        clientSender.send(offerTopic, requestOffers);
    }

    /*
     * Publishes a list of offered goods. Sometimes we publish the list on user's request,
     * sometimes we react to an event.
     *
     * @param sender an (unbound) sender that fits into current session
     */
    private void publishGoodsList(MessageProducer sender, Session session) throws JMSException {
        // TODO Done

        // create a message (of appropriate type) holding the list of offered goods
        // which can be created like this: new ArrayList<Goods>(offeredGoods.values())
        ObjectMessage reply = session.createObjectMessage();

		reply.setStringProperty(CLIENT_MESSAGE_TYPE_KEY, CLIENT_MESSAGE_TYPE_GOODS_LISTING);
        synchronized(offeredGoods) {
			reply.setObject((Serializable) offeredGoods.values().stream().collect(Collectors.toList()));
		}
        // don't forget to include the clientName in the message so other clients know
        // who is sending the offer - see how connect() does it when sending message to bank
		reply.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        // send the message using the sender passed as parameter
        sender.send(offerTopic, reply);
    }

    /*
     * Sends an empty offer and disconnects from the broker.
     */
    private void disconnect() throws JMSException {
        // delete all offered goods
        offeredGoods.clear();

        // send the empty list to indicate client quit
        publishGoodsList(clientSender, clientSession);

        // close the connection to broker
        conn.close();
    }

    /*
     * Prints known goods that are offered by other clients.
     */
    private void list() {
        		synchronized(availableGoods) {

        System.out.println("Available goods (name: price):");
        // iterate over sellers

        for (String sellerName : availableGoods.keySet()) {
            System.out.println("From " + sellerName);
            // iterate over goods offered by a seller
            for (Goods g : availableGoods.get(sellerName)) {
                System.out.println("  " + g);
            }
        }
                }
    }

    /*
     * Main interactive user loop.
     */
    private void loop() throws IOException, JMSException {
        // first connect to broker and setup everything
        connect();

        loop:
        while (true) {
            System.out.println("\nAvailable commands (type and press enter):");
            System.out.println(" l - list available goods");
            System.out.println(" p - publish list of offered goods");
            System.out.println(" b - buy goods");
            System.out.println(" a - print balance");
            System.out.println(" q - quit");
            String line = in.readLine();
            if (line.isEmpty()) {
                continue;
            }
            switch (line.charAt(0)) {
                case 'q':
                    disconnect();
                    break loop;
                case 'b':
                    buy();
                    break;
                case 'l':
                    list();
                    break;
                case 'p':
                    publishGoodsList(clientSender, clientSession);
                    System.out.println("List of offers published");
                    break;
                case 'a':
                    printBalance();
                    break;
                default:
                    break;
            }
        }
    }

    /*
     * Performs buying of goods.
     */
    private void buy() throws IOException, JMSException {
        // get information from the user
        System.out.println("Enter seller name:");
        String sellerName = in.readLine();
        System.out.println("Enter goods name:");
        String goodsName = in.readLine();

        // check if the seller exists
        List<Goods> sellerGoods = availableGoods.get(sellerName);
        if (sellerGoods == null) {
            System.out.println("Seller does not exist: " + sellerName);
            return;
        }
        // check if the goods exists
        Goods goods = sellerGoods.stream().filter(g -> g.name().equals(goodsName)).findFirst().orElse(null);
        if (goods == null) {
            System.out.println("Seller does not offer such goods.");
            return;
        }
        // TODO Done[]

        // First consider what message types clients will use for communicating a sale
        // we will need to transfer multiple values (of String and int) in each message
        // MapMessage? ObjectMessage? TextMessage with extra properties?

        /* Step 1: send a message to the seller requesting the goods */

        // create local reference to the seller's queue
        // similar to Step 2 in connect() but using sellerName instead of clientName
        System.out.println("DEBUG: creating queue for " + sellerName);
		var que = clientSession.createQueue(getClientSaleQueName(sellerName));

        // create message requesting sale of the goods
        // includes: clientName, goodsName, accountNumber
        // also include reply destination that the other client will use to send reply (replyQueue)
        // how? see how connect() uses SetJMSReplyTo()
        // NOTE: we could use custom classes, but let's not complicate it
        System.out.println("DEBUG: generating sale request for " + clientName + " " + goodsName + " " + accountNumber);
        MapMessage saleRequest = clientSession.createMapMessage();
        saleRequest.setJMSReplyTo(replyQueue);
        saleRequest.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        saleRequest.setStringProperty(CLIENT_MESSAGE_TYPE_KEY, CLIENT_MESSAGE_TYPE_OFFER);
        saleRequest.setStringProperty(CLIENT_GOODS_NAME_KEY, goodsName);
        saleRequest.setIntProperty(CLIENT_ACCOUNT_NUMBER_KEY, accountNumber);

        // send the message (with clientSender)
        System.out.println("DEBUG: sending sale request");
        clientSender.send(que, saleRequest);
        /* Step 2: get seller's response and process it */
        
        // receive the reply (synchronously, using replyReceiver)
        System.out.println("DEBUG: waiting for reply");
		ObjectMessage requestReply = (ObjectMessage)replyReceiver.receive();
        var accepted = requestReply.getBooleanProperty(CLIENT_OFFER_ACCEPTED_KEY);
        
        // parse the reply (depends on your selected message format)
        // distinguish between "sell denied" and "sell accepted" message
        // in case of "denied", report to user and return from this method
        // in case of "accepted"
        if  (!accepted) {
            System.out.println("Sale not accepted by seller.");
			return;
        }
        // - obtain seller's account number and price to pay
        var sellerAccount = requestReply.getIntProperty(CLIENT_ACCOUNT_NUMBER_KEY);
        var price = requestReply.getIntProperty(CLIENT_PRICE_KEY);

        /* Step 3: send message to bank requesting money transfer */

        // create message ordering the bank to send money to seller
        MapMessage bankMsg = clientSession.createMapMessage();
        bankMsg.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        bankMsg.setInt(Bank.ORDER_TYPE_KEY, Bank.ORDER_TYPE_SEND);
        bankMsg.setInt(Bank.ORDER_RECEIVER_ACC_KEY, sellerAccount);
        bankMsg.setInt(Bank.AMOUNT_KEY, price);

        bankMsg.setStringProperty(Bank.ORDER_METADATA_KEY, sellerName);

        System.out.println("Sending $" + price + " to account " + sellerAccount);

        // send message to bank
        clientSender.send(toBankQueue, bankMsg);

        /* Step 4: wait for seller's sale confirmation */

        // receive the confirmation, similar to Step 2
        TextMessage confirmation = (TextMessage) replyReceiver.receive();
        String msg = confirmation.getText();

        // parse message and verify its confirmation message
        var confirmed = confirmation.getBooleanProperty(CLIENT_SALE_CONFIRMED);
        if (confirmed) {
			System.out.println("Sale confirmed: " + msg);
		} else {
			System.out.println("Sale not confirmed: " + msg);
		}
        // report successful sale to the user
    }

    /*
     * Processes a message with a goods offer.
     */
    private synchronized void processOffer(Message msg) throws JMSException {
        // parse the message, obtaining sender's name and list of offered goods
        if (msg instanceof TextMessage txtMsg) {
            // Handle request for offers
            if (txtMsg.getText().equals(CLIENT_MESSAGE_TYPE_REQUEST_OFFERS)) {
                // Send our current offers to the requesting client
                publishGoodsList(eventSender, eventSession);
                return;
            }
            else {
                System.out.println("Received unknown message type: " + txtMsg.getText());
                return;
            }
        }
        
        ObjectMessage obj = (ObjectMessage) msg;
        String sender = obj.getStringProperty(CLIENT_NAME_PROPERTY);
        // should ignore messages sent from myself
        if (sender.equals(clientName)) return;
        var lst = (ArrayList<Goods>) obj.getObject();

        // store the list into availableGoods (replacing any previous offer)
        if (lst.isEmpty()) {
            availableGoods.remove(sender);
        } else {
            availableGoods.put(sender, lst);
        }
    }

    /*
     * Processes a message requesting a sale.
     */
    private void processSale(Message msg) throws JMSException {
        // TODO

        /* Step 1: parse the message */

        // distinguish that it's the sale request message
		MapMessage requestMessage = (MapMessage) msg;
        if (requestMessage.getStringProperty(CLIENT_MESSAGE_TYPE_KEY).equals(CLIENT_MESSAGE_TYPE_OFFER_CANCELLATION)){
            System.out.println("Received offer cancellation from " + requestMessage.getStringProperty(CLIENT_NAME_PROPERTY));
            var buyerName = requestMessage.getStringProperty(CLIENT_NAME_PROPERTY);
            Destination buyerDest = reserverDestinations.get(buyerName);

            TextMessage confirmation = eventSession.createTextMessage();
            confirmation.setBooleanProperty(CLIENT_SALE_CONFIRMED, false);
            confirmation.setText("Order cancelled");
            eventSender.send(buyerDest, confirmation);	

            synchronized (reservedGoods) {
                // unreserve the goods
                Goods g = reservedGoods.get(buyerName);
                if (g == null) {
                    System.out.println("No goods reserved for " + buyerName);
                    return;
                }
                offeredGoods.put(g.name(), g);
                reservedGoods.remove(buyerName);
            }
            return;
        }

        if (!requestMessage.getStringProperty(CLIENT_MESSAGE_TYPE_KEY).equals(CLIENT_MESSAGE_TYPE_OFFER))
            {
                System.out.println("Received unknown message type: " + requestMessage.getStringProperty(CLIENT_MESSAGE_TYPE_KEY));
                return;
            }
        // obtain buyer's name (buyerName), goods name (goodsName) , buyer's account number (buyerAccount)
        var buyerName = msg.getStringProperty(CLIENT_NAME_PROPERTY);
        var goodsName = msg.getStringProperty(CLIENT_GOODS_NAME_KEY);
        var buyerAccount = msg.getIntProperty(CLIENT_ACCOUNT_NUMBER_KEY);

        // also obtain reply destination (buyerDest)
        // how? see for example Bank.processTextMessage()
		Destination buyerDest = msg.getJMSReplyTo();

        /* Step 2: decide what to do and modify data structures accordingly */
        synchronized (offeredGoods) {
            
        // check if we still offer these goods
        Goods goods = offeredGoods.get(goodsName);
        var reply = eventSession.createObjectMessage();

        if (goods == null) {
            reply.setBooleanProperty(CLIENT_OFFER_ACCEPTED_KEY, false);
            eventSender.send(buyerDest, reply);
            return;
        }

        // if yes, we should remove it from offeredGoods and publish new list
        // also it's useful to create a list of "reserved goods" together with buyer's information
        // such as name, account number, reply destination
        offeredGoods.remove(goodsName);
        reservedGoods.put(buyerName, goods);
        reserverAccounts.put(buyerAccount, buyerName);
        reserverDestinations.put(buyerName, buyerDest);

        /* Step 3: send reply message */

        // prepare reply message (accept or deny)
        // accept message includes: my account number (accountNumber), price (goods.price)
            reply.setBooleanProperty(CLIENT_OFFER_ACCEPTED_KEY, true);
            reply.setIntProperty(CLIENT_PRICE_KEY, goods.price());
            reply.setIntProperty(CLIENT_ACCOUNT_NUMBER_KEY, accountNumber);

			// send reply
			eventSender.send(buyerDest, reply);
        }

    }

    /*
     * Processes message with (transfer) report from the bank.
     */
    private synchronized void processBankReport(Message msg) throws JMSException {
        /* Step 1: parse the message */

        // Bank reports are sent as MapMessage
        if (msg instanceof MapMessage) {
            MapMessage mapMsg = (MapMessage) msg;
            // get report number
            int cmd = mapMsg.getInt(Bank.REPORT_TYPE_KEY);
            if (cmd == Bank.REPORT_TYPE_RECEIVED) {
                // get account number of sender and the amount of money sent
                int buyerAccount = mapMsg.getInt(Bank.REPORT_SENDER_ACC_KEY);
                int amount = mapMsg.getInt(Bank.AMOUNT_KEY);
// get the buyer's destination
                String buyerName = reserverAccounts.get(buyerAccount);

                if (buyerName == null) {
                    System.out.println("Received money from unknown account " + buyerAccount + ", returning $" + amount);
                    returnMoney(buyerAccount, amount);
                    return;
                }
                Destination buyerDest = reserverDestinations.get(buyerName);

                // match the sender account with sender

                // match the reserved goods
                Goods g = reservedGoods.get(buyerName);

                System.out.println("Received $" + amount + " from " + buyerName);

                /* Step 2: decide what to do and modify data structures accordingly */

                // did they pay enough?
                if (amount == g.price()) {
                    
                    // remove the reserved goods and buyer-related information
                    reserverDestinations.remove(buyerName);
                    reserverAccounts.remove(buyerAccount);
                    reservedGoods.remove(buyerName);

                    /* TODO Step 3: send confirmation message */
                    // prepare sale confirmation message
                    // includes: goods name (g.name)
                    TextMessage confirmation = eventSession.createTextMessage();
                    confirmation.setBooleanProperty(CLIENT_SALE_CONFIRMED, true);
                    confirmation.setText(g.name());
                    // prepare sale confirmation message
                    // includes: goods name (g.name)

                    // send reply (destination is buyerDest)
                    eventSender.send(buyerDest, confirmation);	

                } else {
                    /* TODO Step 4: the buyer did not pay enough */
                    // reoffer the reserved goods and reply to the buyer
                    offeredGoods.put(g.name(), g);
                    reservedGoods.remove(buyerName);
                    if (amount > 0) {
                        returnMoney(buyerAccount, amount);
                    }

						// Tell the buyer the order is cancelled
                    TextMessage confirmation = eventSession.createTextMessage();
                    confirmation.setBooleanProperty(CLIENT_SALE_CONFIRMED, false);
                    confirmation.setText("Invalid payment. Paid $" + amount + " instead of $" + g.price() + ". Payment will be wired back.");
                    eventSender.send(buyerDest, confirmation);	

                } 
            } else if (cmd == Bank.REPORT_TYPE_INSUFFICIENT_BALANCE) {
                // cancel the sale
                // send to the seller the message that the sale is cancelled

                var sellerName = mapMsg.getStringProperty(Bank.ORDER_METADATA_KEY);
                if (sellerName == null) {
                    System.out.println("Received insufficient balance report for transaction with unknown seller");
                    return;
                }
                System.out.println("Received insufficient balance report for transaction with seller" + sellerName);
                var que = clientSession.createQueue(getClientSaleQueName(sellerName));

                System.out.println("Sending cancellation to " + sellerName);
                MapMessage cancellation = clientSession.createMapMessage();
                cancellation.setStringProperty(CLIENT_MESSAGE_TYPE_KEY, CLIENT_MESSAGE_TYPE_OFFER_CANCELLATION);
                cancellation.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
                clientSender.send(que, cancellation);
            }
            else {
                System.out.println("Received unknown MapMessage:\n: " + msg);
            }
        } else {
            System.out.println("Received unknown message:\n: " + msg);
        }
    }

    /*
     * Main method, creates client instance and runs its loop
     */
    public static void main(String[] args) throws JMSException, IOException {
        if (args.length != 1) {
            System.err.println("Usage: ./client <clientName>");
            return;
        }

        // create connection to the broker.
        try (ActiveMQConnectionFactory connectionFactory = new ActiveMQConnectionFactory("tcp://localhost:61616");
             Connection connection = connectionFactory.createConnection()) {
            // create instance of the client
            Client client = new Client(args[0], connection);

            // perform client loop
            client.loop();
        }
    }

    private String getClientSaleQueName(String clientName) {
        return clientName + "SaleQueue";
    }

    private void returnMoney(int buyerAccount, int amount) throws JMSException {
        MapMessage bankMsg = eventSession.createMapMessage();
        bankMsg.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        bankMsg.setInt(Bank.ORDER_TYPE_KEY, Bank.ORDER_TYPE_SEND);
        bankMsg.setInt(Bank.ORDER_RECEIVER_ACC_KEY, buyerAccount);
        bankMsg.setInt(Bank.AMOUNT_KEY, amount);
        eventSender.send(toBankQueue, bankMsg);
    }

    private void printBalance() throws JMSException {
        Queue q = clientSession.createTemporaryQueue();
        MessageConsumer balanceReceiver = clientSession.createConsumer(q);

        TextMessage balanceRequest = clientSession.createTextMessage(Bank.CHECK_BALANCE_MSG);
        balanceRequest.setStringProperty(CLIENT_NAME_PROPERTY, clientName);
        balanceRequest.setIntProperty(Bank.ACCOUNT_NUMBER_KEY, accountNumber);
        balanceRequest.setJMSReplyTo(q);
        clientSender.send(toBankQueue, balanceRequest);

        TextMessage reply = (TextMessage) balanceReceiver.receive();
        if (reply.getText() != null && reply.getText().equals("Account does not exist")) {
            System.out.println("Error: Account does not exist");
        } else {
            int balance = reply.getIntProperty(Bank.BALANCE_KEY);
            System.out.println("Current balance: $" + balance);
        }

        balanceReceiver.close();
    }
}
