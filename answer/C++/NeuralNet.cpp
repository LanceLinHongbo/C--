import std;

// 定义relu函数
auto relu(double x) -> double
{
    return x > 0 ? x : 0.01 * x;
}

// 定义relu函数导数
auto relu_d(double x) -> double
{
    return x > 0 ? 1.0 : 0.01;
}

// 定义sigmoid函数
auto sigmoid(double x) -> double
{
    return 1 / (1 + std::exp<double>(-x));
}

// 定义sigmoid函数导数
auto sig_d(double x) -> double
{
    double s = sigmoid(x);
    return s * (1 - s);
}

class NeuralNet
{
  public:
    // 构造函数，初始化权重和偏置
    NeuralNet(double w1_, double w2_, double b1_, double b2_)
        : w1(w1_)
        , w2(w2_)
        , b1(b1_)
        , b2(b2_)
    {}

    // 前向传播函数
    [[nodiscard]] auto forward(double input,
                               double target) const -> std::array<double, 5>
    {
        double z1 = input * w1 + b1;
        double h = relu(z1);
        double z2 = h * w2 + b2;
        double output = sigmoid(z2);
        double loss = std::pow<double>(target - output, 2); // MSE损失
        std::cout << "input: " << input << "  output: " << output << "\n";
        return {z1, h, z2, output, loss};
    }

    // 推导函数
    [[nodiscard]] auto derive(double input) const -> double
    {
        double z1 = input * w1 + b1;
        double h = relu(z1);
        double z2 = h * w2 + b2;
        double output = sigmoid(z2);
        return output;
    }

    // 反向传播函数
    void backward(double input, double target, double ln_rt)
    {
        auto res = forward(input, target);
        auto z1 = res[0];
        auto h = res[1];
        auto z2 = res[2];
        auto output = res[3];

        // 计算输出层的梯度
        double d_z2 = 2 * (output - target) * sig_d(z2);
        double d_w2 = d_z2 * h;
        double d_b2 = d_z2;

        // 计算隐藏层的梯度
        double d_z1 = d_z2 * w2 * relu_d(z1);
        double d_w1 = d_z1 * input;
        double d_b1 = d_z1;

        // 更新输出层的权重和偏置
        w2 -= ln_rt * d_w2;
        b2 -= ln_rt * d_b2;

        // 更新隐藏层的权重和偏置
        w1 -= ln_rt * d_w1;
        b1 -= ln_rt * d_b1;

        std::cout << "d_w1 = " << d_w1 << " " << "d_b1 = " << d_b1 << "\n"
                  << "d_w2 = " << d_w2 << " " << "d_b2 = " << d_b2 << "\n";

        std::cout << "w1 = " << w1 << " " << "b1 = " << b1 << "\n"
                  << "w2 = " << w2 << " " << "b2 = " << b2 << "\n";
    }

    [[nodiscard]] auto getW1() const -> double
    {
        return w1;
    }

    [[nodiscard]] auto getW2() const -> double
    {
        return w2;
    }

    [[nodiscard]] auto getB1() const -> double
    {
        return b1;
    }

    [[nodiscard]] auto getB2() const -> double
    {
        return b2;
    }

  private:
    // 权重
    double w1;
    double w2;
    // 偏置
    double b1;
    double b2;
};

auto main() -> int
{
    NeuralNet net(1, 1, 1, 1);
    double input[100];
    double target[100];
    double ln_rt = 5;
    std::srand(std::time(nullptr));

    // 进行反向传播调整
    for (int i = 0; i != 100; ++i)
    {
        double r = (double)std::rand() / 0x7fff;

        if (i % 2 == 0)
        {
            input[i] = r;
        }
        else
        {
            input[i] = -r;
        }
        if (input[i] < 0)
        {
            target[i] = 0;
        }
        else
        {
            target[i] = 1;
        }
        net.backward(input[i], target[i], ln_rt);
    }

    // 输出更新后的权重和偏置
    std::cout << "Updated weights and biases: " << std::endl;
    std::cout << "w1: " << net.getW1() << ", w2: " << net.getW2() << std::endl;
    std::cout << "b1: " << net.getB1() << ", b2: " << net.getB2() << std::endl;

    double res;
    while (std::cin >> res)
    {
        std::cout << net.derive(res) << "\n";
    }

    return 0;
}