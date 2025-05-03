package cz.cuni.mff.d3s.nswi080.anomaly.analyzer;

/**
 * Describes a detected anomaly.
 */
public record Anomaly(String hostname, String description, long timestamp) {
}
