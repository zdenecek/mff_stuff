package cz.cuni.mff.d3s.nswi080.anomaly.analyzer;

import cz.cuni.mff.d3s.nswi080.anomaly.JacksonKafkaDeserializer;
import java.util.Map;

/**
 * The value stored in the {@link Analyzer#ANALYZER_STATE_TOPIC}.
 */
public record AnalyzerStateValue(Map<String, AnomalyAnalyzer> analyzers, long offset) {
    public static final class AnalyzerStateValueDeserializer extends JacksonKafkaDeserializer<AnalyzerStateValue> {
        /**
         * The constructor must be public.
         */
        public AnalyzerStateValueDeserializer() {
            super(AnalyzerStateValue.class);
        }
    }
}
