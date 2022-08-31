#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <utility>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hittable_list : public hittable
{
public:
	std::vector<shared_ptr<hittable>> objects;

	hittable_list() = default;
	explicit hittable_list(shared_ptr<hittable> object) { add(std::move(object)); }

	void clear() { objects.clear(); }
	void add(const shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, float t_min, float t_max, hit_record& rec) const override;
};

inline bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& rec) const {
	hit_record temp_rec;
	bool has_hit_anything = false;
	auto closest_so_far = t_max;

	for (const auto& object : objects)
	{
		if (object->hit(r, t_min, closest_so_far, temp_rec))
		{
			has_hit_anything = true;
			closest_so_far = temp_rec.t;
			rec = temp_rec;
		}
	}

	return has_hit_anything;
}

#endif