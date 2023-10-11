#pragma once
#include "collision_grid.hpp"
#include "PhysicObject.hpp"
#include "utils.hpp"
#include "index_vector.hpp"


struct PhysicSolver
{
	CIVector<PhysicObject> objects;
	CollisionGrid          grid;
	glm::vec2              world_size;
	glm::vec2              gravity = { 0.0f, -100.0f };

	// Simulation solving pass count
	uint32_t        sub_steps;
	//tp::ThreadPool& thread_pool;

	PhysicSolver(glm::vec2 size)
		: grid{ (int32_t)size.x, (int32_t)size.y }
		, world_size{ to<float>(size.x), to<float>(size.y) }
		, sub_steps{ 8 }
	{
		grid.clear();
	}

	~PhysicSolver() 
	{
		objects.clear();
		grid.clear();
	}

	// Checks if two atoms are colliding and if so create a new contact
	void solveContact(uint32_t atom_1_idx, uint32_t atom_2_idx)
	{
		constexpr float response_coef = 1.0f;
		constexpr float eps = 0.0001f;
		PhysicObject& obj_1 = objects.data[atom_1_idx];
		PhysicObject& obj_2 = objects.data[atom_2_idx];
		const glm::vec2 o2_o1 = obj_1.position - obj_2.position;
		const float dist2 = o2_o1.x * o2_o1.x + o2_o1.y * o2_o1.y;
		if (dist2 < 1.0f && dist2 > eps) {
			const float dist = sqrt(dist2);
			// Radius are all equal to 1.0f
			const float delta = response_coef * 0.5f * (1.0f - dist);
			const glm::vec2 col_vec = (o2_o1 / dist) * delta;
			obj_1.position += col_vec;
			obj_2.position -= col_vec;
		}
	}

	void checkAtomCellCollisions(uint32_t atom_idx, const CollisionCell& c)
	{
		for (uint32_t i{ 0 }; i < c.objects_count; ++i) {
			solveContact(atom_idx, c.objects[i]);
		}
	}

	void processCell(const CollisionCell& c, uint32_t index)
	{
		for (uint32_t i{ 0 }; i < c.objects_count; ++i) {
			const uint32_t atom_idx = c.objects[i];
			checkAtomCellCollisions(atom_idx, grid.data[index - 1]);
			checkAtomCellCollisions(atom_idx, grid.data[index]);
			checkAtomCellCollisions(atom_idx, grid.data[index + 1]);
			checkAtomCellCollisions(atom_idx, grid.data[index + grid.height - 1]);
			checkAtomCellCollisions(atom_idx, grid.data[index + grid.height]);
			checkAtomCellCollisions(atom_idx, grid.data[index + grid.height + 1]);
			checkAtomCellCollisions(atom_idx, grid.data[index - grid.height - 1]);
			checkAtomCellCollisions(atom_idx, grid.data[index - grid.height]);
			checkAtomCellCollisions(atom_idx, grid.data[index - grid.height + 1]);
		}
	}

	void solveCollisionThreaded(uint32_t i, uint32_t slice_size)
	{
		const uint32_t start = i * slice_size;
		const uint32_t end = (i + 1) * slice_size;
		for (uint32_t idx{ start }; idx < end; ++idx) {
			processCell(grid.data[idx], idx);
		}
	}

	// Find colliding atoms
	void solveCollisions()
	{
		//// Multi-thread grid
		//const uint32_t thread_count = thread_pool.m_thread_count;
		//const uint32_t slice_count  = thread_count * 2;
		//const uint32_t slice_size   = (grid.width / slice_count) * grid.height;
		// Find collisions in two passes to avoid data races
		// First collision pass

		for (uint32_t i{ 0 }; i < grid.data.size(); ++i)
		{
			processCell(grid.data[i], i);
		}
	}

	// Add a new object to the solver
	uint64_t addObject(const PhysicObject& object)
	{
		return objects.push_back(object);
	}

	// Add a new object to the solver
	uint64_t createObject(glm::vec2 pos)
	{
		return objects.emplace_back(pos);
	}

	void update(float dt)
	{
		// Perform the sub steps
		const float sub_dt = dt / static_cast<float>(sub_steps);
		for (uint32_t i(sub_steps); i--;) {
			addObjectsToGrid();
			solveCollisions();
			updateObjects_multi(sub_dt);
		}
	}

	void addObjectsToGrid()
	{
		grid.clear();
		// Safety border to avoid adding object outside the grid
		uint32_t i{ 0 };
		for (const PhysicObject& obj : objects.data) {
			if (obj.position.x > 1.0f && obj.position.x < world_size.x - 1.0f &&
				obj.position.y > 1.0f && obj.position.y < world_size.y - 1.0f) {
				grid.addAtom(to<int32_t>(obj.position.x), to<int32_t>(obj.position.y), i);
			}
			++i;
		}
	}

	void updateObjects_multi(float dt)
	{
		for (uint32_t i{ 0 }; i < objects.size(); ++i)
		{
			PhysicObject& obj = objects.data[i];
			// Add gravity
			obj.acceleration += gravity;
			// Apply Verlet integration
			obj.update(dt);
			// Apply map borders collisions
			const float margin = 2.0f;
			if (obj.position.x > world_size.x - margin) 
				obj.position.x = world_size.x - margin;
			
			else if (obj.position.x < margin) 
				obj.position.x = margin;
			
			if (obj.position.y > world_size.y - margin) 
				obj.position.y = world_size.y - margin;
			
			else if (obj.position.y < margin) 
				obj.position.y = margin;
			
		}
	}
};
