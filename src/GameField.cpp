#include "GameField.h"

#include <random>

struct pos_t
{
    size_t row;
    size_t col;

    bool operator!=(const pos_t& other)
    {
        bool ret = false;

        if (this->col != other.col || this->row != other.row)
        {
            ret = true;
        }

        return ret;
    }
};

pos_t getPosFromIndex(const size_t& index)
{
    pos_t pos;
    pos.row = index / 4;
    pos.col = index - pos.row * 4;

    return pos;
}

size_t getIndexFromPos(const pos_t& pos)
{
    return pos.row * 4 + pos.col;
}

GameField::GameField()
    :_gameField(), _isChanged(true), _zeroPos(_gameField.size() - 1),
    _isGameCompleted(false)
{
    reload();
}

GameField::~GameField()
{
}

void GameField::move(const MoveDirection& direction)
{
    pos_t pos = getPosFromIndex(_zeroPos);
    pos_t newPos = pos;

    if (direction == MoveDirection::left && pos.col > 0)
    {
        --newPos.col;
    }
    else if (direction == MoveDirection::right && pos.col < 3)
    {
        ++newPos.col;
    }
    else if (direction == MoveDirection::up && pos.row > 0)
    {
        --newPos.row;
    }
    else if (direction == MoveDirection::down && pos.row < 3)
    {
        ++newPos.row;
    }

    if (newPos != pos)
    {
        size_t newZeroPos = getIndexFromPos(newPos);
        std::swap(_gameField[_zeroPos], _gameField[newZeroPos]);
        _zeroPos = newZeroPos;
        _isChanged = true;
    }

}

void GameField::reload()
{
    _isGameCompleted = false;
    _zeroPos = _gameField.size() - 1;
    for(uint8_t i = 0; i < _gameField.size(); ++i)
    {
        _gameField[i] = i + 1;
        if (i == _gameField.size() - 1)
        {
            _gameField[i] = 0;
        }
    }

    std::random_device rd;
    std::mt19937 g(rd());
    std::uniform_int_distribution<size_t> gen(0, 3);
    for(size_t i = 0; i < 16*16; ++i)
    {
        MoveDirection dir = static_cast<MoveDirection>(gen(g));
        move(dir);
    }
}

void GameField::update()
{
    if (this->isFieldChanged())
    {
        recalculateGameCompletedFlag();
    }
    _isChanged = false;
}

const bool& GameField::isGameCompleted() const
{
    return _isGameCompleted;
}

void GameField::recalculateGameCompletedFlag()
{
    _isGameCompleted = true;
    if (_zeroPos != _gameField.size() - 1)
    {
        _isGameCompleted = false;
    }
    else
    {
        for (size_t i = 0; i < _gameField.size() - 1; ++i)
        {
            if (_gameField[i] != i + 1)
            {
                _isGameCompleted = false;
                break;
            }
        }
    }
}