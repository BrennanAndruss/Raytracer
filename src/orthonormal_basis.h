class OrthonormalBasis
{
public:
	OrthonormalBasis(const Vec3& normal)
	{
		axis[2] = normalize(normal);
		Vec3 a = (std::fabs(axis[2].x) > 0.9f) ?
			Vec3(0.0f, 1.0f, 0.0f) : Vec3(1.0f, 0.0f, 0.0f);
		axis[1] = normalize(cross(axis[2], a));
		axis[0] = cross(axis[2], axis[1]);
	}

	const Vec3& u() const { return axis[0]; }
	const Vec3& v() const { return axis[1]; }
	const Vec3& w() const { return axis[2]; }

	Vec3 transform(const Vec3& v) const
	{
		// Transform from basis coordinates to local space
		return (v[0] * axis[0]) + (v[1] * axis[1]) + (v[2] * axis[2]);
	}

private:
	Vec3 axis[3];
};