#include <iostream>
#include <limits>

class IStatistics {
public:
	virtual ~IStatistics() {}

	virtual void update(double next) = 0;
	virtual double eval() const = 0;
	virtual const char * name() const = 0;
};

class Max : public IStatistics {
private:
	double maxValue = std::numeric_limits<double>::lowest();

public:
	void update(double value) override {
		if (value > maxValue) {
			maxValue = value;
		}
	}

	double eval() const override {
		return maxValue;
	}

	const char* name() const override {
		return "max";
	}
};

class Min : public IStatistics {
public:
	Min() : m_min{std::numeric_limits<double>::max()} {
	}

	void update(double next) override {
		if (next < m_min) {
			m_min = next;
		}
	}

	double eval() const override {
		return m_min;
	}

	const char * name() const override {
		return "min";
	}

private:
	double m_min;
};

class Mean : public IStatistics {
private:
	double sum = 0;
	int count = 0;

public:
	void update(double value) override {
		sum += value;
		++count;
	}

	double eval() const override {
		return count > 0 ? sum / count : 0; //предотвращение деления на ноль
	}

	const char* name() const override {
		return "mean";
	}
};

class Std : public IStatistics {
private:
	double sum = 0;
	double sumOfSquares = 0;
	int count = 0;

public:
	void update(double value) override {
		sum += value;
		sumOfSquares += value * value;
		++count;
	}

	double eval() const override {
		if (count > 1) {
			double mean = sum / count;
			double variance = (sumOfSquares - 2 * mean * sum + count * mean * mean) / count;
			return std::sqrt(variance);
		}
		return 0;
	}

	const char* name() const override {
		return "std";
	}
};

int main() {

	const size_t statistics_count = 4;
	IStatistics *statistics[statistics_count];

	statistics[0] = new Min{};
	statistics[1] = new Max{};
	statistics[2] = new Mean{};
	statistics[3] = new Std{};

	double val = 0;
	while (std::cin >> val) {
		for (size_t i = 0; i < statistics_count; ++i) {
			statistics[i]->update(val);
		}
	}

	// Handle invalid input data
	if (!std::cin.eof() && !std::cin.good()) {
		std::cerr << "Invalid input data\n";
		return 1;
	}

	// Print results if any
	for (size_t i = 0; i < statistics_count; ++i) {
		std::cout << statistics[i]->name() << " = " << statistics[i]->eval() << std::endl;
	}

	// Clear memory - delete all objects created by new
	for (size_t i = 0; i < statistics_count; ++i) {
		delete statistics[i];
	}

	return 0;
}