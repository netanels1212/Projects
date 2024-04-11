#include <assert.h> /* assert */
#include <stdio.h> /* printf */
                                                    
#include "bitarray.h"


#define DIRECTIONS  8
#define ALL_VISITED ~0U

typedef struct
{
	int x;
	int y;
} pos_ty;


static size_t PositionToIdxIMP(pos_ty position_)
{
	/* return the correct idx according to x and y */
    return (size_t)position_.x + (size_t)position_.y * 8;
}

static int AllPositionsTakenIMP(size_t board_)
{
	/* return if all positions mark as visited */
	return (ALL_VISITED == board_);
}

static size_t SetPositionAsTakenIMP(size_t board_, size_t idx_)
{
	/* set idx_ to visited form */
	/*return SetOn(board_, idx_); - segmentation fault.. can't understand why..*/
	return board_ | (1U << idx_);	 
}

static int IsPositionTakenIMP(size_t board_, size_t idx_)
{
	/* return if this idx_ visited */
	return (board_ & (1U << idx_));
}

static int IsOnBoardIMP(pos_ty position_)
{
	/* if x,y isn't in the correct range */
	if (0 > position_.x || position_.x > 7 || 0 > position_.y || position_.y > 7)
	{
		/* return false */
		return 0;
	}

	/* return true */
	return 1;
}

static pos_ty NextPositionIMP(pos_ty position_, size_t direction_)   
{
    static const pos_ty directions[DIRECTIONS] = {{1,2}, {2,1}, {2,-1}, {1,-2}, {-1,-2}, {-2,-1}, {-2,1}, {-1,2}};
    pos_ty ret;

	/* assert */
	assert(DIRECTIONS > direction_);

	/* set x,y on ret to indicate the new position after move in the direction_ */
	ret.x = position_.x + directions[direction_].x;
	ret.y = position_.y + directions[direction_].y;

	/* return ret */
	return ret;
}

static int CanVisitIMP(size_t board_, pos_ty position_)
{
	pos_ty next_pos = {0, 0};
	size_t pos_idx = PositionToIdxIMP(position_);
	size_t i = 0;

	/* if position mark as visited or it out of board */
	if (!IsOnBoardIMP(position_) || IsPositionTakenIMP(board_, pos_idx))
	{
		/* return false */
		return 0;
	}

	/* mark position on board */
	board_ = SetPositionAsTakenIMP(board_, pos_idx);

	/* if AllVisitedIMP(board) */
	if (AllPositionsTakenIMP(board_))
	{
		/* return true */
		return 1;
	}

	/* for all(8) directions */
	for (i = 0; i < DIRECTIONS; ++i)
	{
		/* next_pos = NextPosition(position direction) */
		next_pos = NextPositionIMP(position_, i);

		/* if CanVisitIMP(board, next_pos) */
		if (CanVisitIMP(board_, next_pos))
		{
			/* return true */
			return 1;
		}
	}

	/* return false */
	return 0;
}

int KnightTour(int x, int y)
{
	size_t board = 0;
	pos_ty start_pos;

	/* position = {x,y} */
	start_pos.x = x;
	start_pos.y = y;

	/* assert */
	assert(IsOnBoardIMP(start_pos));

	/* return CanVisitIMP(board, position) */
	return CanVisitIMP(board, start_pos);
}

static int CanVisitP2IMP(size_t board_, pos_ty position_, int steps_[][2])
{
	pos_ty next_pos = {0, 0};
	size_t pos_idx = PositionToIdxIMP(position_);
	size_t i = 0;

	/* if position mark as visited or it out of board */
	if (!IsOnBoardIMP(position_) || IsPositionTakenIMP(board_, pos_idx))
	{
		/* return false */
		return 0;
	}

	/* mark position on board */
	board_ = SetPositionAsTakenIMP(board_, pos_idx);


	/* if AllVisitedIMP(board) */
	if (AllPositionsTakenIMP(board_))
	{
		/* save curr position in steps_ */
		*steps_[0] = position_.x;
		*steps_[1] = position_.y;

		/* return true */
		return 1;
	}

	/* for all(8) directions */
	for (i = 0; i < DIRECTIONS; ++i)
	{
		/* next_pos = NextPosition(position direction) */
		next_pos = NextPositionIMP(position_, i);

		/* if CanVisitP2IMP(board, next_pos) */
		if (CanVisitP2IMP(board_, next_pos, steps_ + 1))
		{
			/* save curr position in steps_ */
			*steps_[0] = position_.x;
			*steps_[1] = position_.y;

			/* return true */
			return 1;
		}
	}

	/* return false */
	return 0;
}

int KnightTourP2(int x, int y, int steps_[][2])
{
	size_t board = 0;
	pos_ty start_pos;

	/* position = {x,y} */
	start_pos.x = x;
	start_pos.y = y;

	/* assert */
	assert(IsOnBoardIMP(start_pos));

	/* return CanVisitP2IMP(board, position) */
	return CanVisitP2IMP(board, start_pos, steps_);
}

static void PrintStepsIMP(int steps_[][2])
{
	size_t i = 0;

	/* assert */
	assert(NULL != steps_);

	/* print steps */
	while (64 > i)
	{
		printf("S%lu: x=%d, y=%d\n", i, *steps_[0], *steps_[1]);
		++steps_;
		++i;
	}
}

int main()
{
	int x = 0;
	int y = 0;
	int steps[64][2] = {0};


	printf("KnightTour return value for x=%d and y=%d is: %d\n", x, y, KnightTourP2(x, y, steps));
	PrintStepsIMP(steps);


	return 0;
}