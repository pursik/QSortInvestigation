#include <vector>
#include <random>

class UniformRandomGenerator
{
	std::mt19937 random_engine{ std::random_device()() };
public:
	std::vector<double> GenerateVectorOfRandom(double left, double right, int randomNumbers)
	{
		auto distribution = std::uniform_real_distribution<double>(left, right);

		std::vector<double> data(randomNumbers);

		std::generate_n(data.begin(), randomNumbers, [&]()
			{
				return distribution(random_engine);
			});
		return data;
	}
};
