package cz.cuni.mff.d3s.nswi080.documents;

import com.hazelcast.config.ClasspathYamlConfig;
import com.hazelcast.config.Config;
import com.hazelcast.core.Hazelcast;
import com.hazelcast.core.HazelcastInstance;

/**
 * Server component that starts a Hazelcast node using YAML configuration.
 */
public class Member {
    public static void main(String[] args) {
        try {
            Config config = new ClasspathYamlConfig("hazelcast.yaml");
            
            HazelcastInstance hazelcastInstance = Hazelcast.newHazelcastInstance(config);
            
            System.out.println("Hazelcast server node started");
            System.out.println("Press Ctrl+C to exit");
        } catch (Exception e) {
            System.err.println("Error starting Hazelcast server: " + e.getMessage());
            e.printStackTrace();
            System.exit(1);
        }
    }
} 