package cz.cuni.mff.d3s.nswi080.trading;

import java.io.Serial;
import java.io.Serializable;

public record Goods(String name, int price) implements Serializable {
    @Serial
    private static final long serialVersionUID = 8409458516343803478L;

    @Override
    public String toString() {
        return name + ": $" + price;
    }
}
