#include <allegro5/allegro.h>
#include "GridPathfinder.h"
#include "GridGraph.h"
#include "GridVisualizer.h"
#include "Path.h"
#include "Game.h"
#include "GameApp.h"
#include "GraphicsBuffer.h"

GridPathfinder::GridPathfinder( GridGraph* pGraph )
:Pathfinder(pGraph)
,mTimeElapsed(0.0)
,mPathType("")
{
#ifdef VISUALIZE_PATH
	mpVisualizer = NULL;
#endif
}

GridPathfinder::~GridPathfinder()
{
#ifdef VISUALIZE_PATH
	delete mpVisualizer;
#endif
}

#ifdef VISUALIZE_PATH
void GridPathfinder::drawVisualization( Grid* pGrid, GraphicsBuffer* pDest )
{
	GameApp* pGame = dynamic_cast<GameApp*>(gpGame);
	if (mpVisualizer == NULL)
	{
		mpVisualizer = new GridVisualizer(pGrid);
	}
	else
	{
		mpVisualizer->clear();
	}

	static ALLEGRO_COLOR pathColor = al_map_rgb( 255, 64, 64 );
	switch (pGame->getType())
	{
	case DepthBreadthSearch:
		pathColor = al_map_rgb(25, 225, 25);
		break;
	case DijkstraPath:
		 pathColor = al_map_rgb(255, 64, 64);
		break;
	case AstarPath:
		 pathColor = al_map_rgb(64, 64, 255);
		break;

	default:
		break;
	}
	
	

	static ALLEGRO_COLOR startColor = al_map_rgb(1, 255, 128);
	static ALLEGRO_COLOR stopColor = al_map_rgb(100, 15, 140);

	unsigned int numNodes = mPath.getNumNodes();
	ALLEGRO_COLOR currentPathColor = pathColor;

	if (numNodes > 0)
	{
		for (unsigned int i = 1; i < numNodes-1; i++)
		{
			mpVisualizer->addColor(mPath.peekNode(i)->getId(), currentPathColor);
			float lerpVal = lerp( i, 0, numNodes );
			currentPathColor.g = 1.0f - lerpVal;
			
		}

		//add beginning and ending color
		mpVisualizer->addColor(mPath.peekNode(0)->getId(), startColor);
		mpVisualizer->addColor(mPath.peekNode(numNodes - 1)->getId(), stopColor);
	}


	mpVisualizer->draw(*pDest);
}
#endif