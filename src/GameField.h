#pragma once
#include <cstdint>
#include <array>

class GameField
{
private:
    typedef std::array<uint16_t, 16>::_Myt _ArrayType;
    _ArrayType _gameField;
    bool _isChanged;
    bool _isGameCompleted;
    size_t _zeroPos;

private:
    void recalculateGameCompletedFlag();

public:
    enum class MoveDirection
    {
        up,
        down,
        left,
        right
    };

    GameField();
    ~GameField();

    void reload();
    void update();
    void move(const MoveDirection& direction);

    const bool& isGameCompleted() const;

    const _ArrayType& getBasicArray() const
    {
        return _gameField;
    }

    const bool& isFieldChanged() const
    {
        return _isChanged;
    }
};
