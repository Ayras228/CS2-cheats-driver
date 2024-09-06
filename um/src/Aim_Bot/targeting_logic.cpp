
#include <iostream>
#include <vector>


#include "../Vectors/Vectors.h"

inline Vector3 findClosest(const std::vector<Vector3> playerPositions, Vector2 center_of_screen)
{
	if (playerPositions.empty())
	{
		printf("playerPositions vector was empty.\n");
		return { 0.0f, 0.0f, 0.0f };
	}

	float lowestDistance = 10000;
	int index = -1;

	
	for (int i = 0; i < playerPositions.size(); ++i)
	{
		float distance(std::pow(playerPositions[i].get_x() - center_of_screen.get_x(), 2) 
			+ std::pow(playerPositions[i].get_y() - center_of_screen.get_y(), 2));

		if (distance < lowestDistance) 
		{
			lowestDistance = distance;
			index = i;
		}
	}

	
	if (index == -1) {
		return { 0.0f, 0.0f, 0.0f };
	}

	
	return { playerPositions[index].get_x(), playerPositions[index].get_y(), 0.0f };
}

inline void MoveMouseToPlayer(Vector3 &position, Vector2 &center_of_screen)
{
	
	if (position.IsZero())
		return;

	// get our new x and y, by subtracting the position by the center of the screen,
	// giving us a position to move the mouse to.
	auto new_x = position.get_x() - center_of_screen.get_x();
	auto new_y = position.get_y() - center_of_screen.get_y();

	// move the mouse to said position.
	mouse_event(MOUSEEVENTF_MOVE, new_x, new_y, 0, 0);
}