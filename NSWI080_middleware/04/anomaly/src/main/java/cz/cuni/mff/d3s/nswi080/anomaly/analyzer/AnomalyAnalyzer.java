package cz.cuni.mff.d3s.nswi080.anomaly.analyzer;

import java.time.Duration;
import java.util.LinkedList;

/**
 * Stateful algorithm that finds anomalies in measurements.
 * <p>
 * You probably do not need to modify this class.
 *
 * @param hostname the hostname of the monitor device
 * @param window a sliding window of the last WINDOW_SIZE measurements
 */
record AnomalyAnalyzer(String hostname, LinkedList<Double> window) {
    /**
     * The size of the sliding window.
     */
    private static final int WINDOW_SIZE = 7;

    /**
     * The threshold (ratio from the window's median) to consider a temperature anomalous.
     */
    private static final double THRESHOLD = 0.2;

    /**
     * We simulate work of the algorithm by sleeping for the given amount.
     */
    private static final Duration SIMULATED_WORKLOAD = Duration.ofSeconds(1);

    public AnomalyAnalyzer(String hostname) {
        this(hostname, new LinkedList<>());
    }

    /**
     * Adds a measurement (updating the internal state) and detects whether the new measurement is an anomaly.
     *
     * @param temperature the measured temperature
     * @param timestamp the timestamp of the measurement
     * @return an anomaly if the measurement is anomalous, {@code null} otherwise
     */
    public Anomaly addRecordAndDetect(double temperature, long timestamp) {
        try {
            Thread.sleep(SIMULATED_WORKLOAD);
        } catch (InterruptedException ignored) {
        }
        if (window.size() < WINDOW_SIZE) {
            window.add(temperature);
            return null;
        }
        window.add(temperature);
        while (window.size() > WINDOW_SIZE) {
            window.removeFirst();
        }
        double median = window.stream().mapToDouble(Double::doubleValue).sorted().toArray()[WINDOW_SIZE / 2];
        if (temperature > (1 + THRESHOLD) * median || temperature < (1 - THRESHOLD) * median) {
            return new Anomaly(hostname, String.format("Value %s is outside the threshold (median: %.2f)", temperature, median), timestamp);
        }
        return null;
    }
}
