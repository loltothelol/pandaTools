#pragma once

#include <memory>

#include <glm/mat4x4.hpp>
#include <glm/vec2.hpp>

namespace Geom
{
	struct Rect
	{
		glm::vec2 m_vertices[4];

		void set(const glm::vec2 * vertices);
	};

	class Bounds
	{
	public:
		static Bounds fromOffsetAndSize(glm::vec2 offset, glm::vec2 size);

		Bounds();
		Bounds(const glm::vec2 & min, const glm::vec2 & max);
		Bounds(const Rect &);

		const glm::vec2 & min() const;
		const glm::vec2 & max() const;

		glm::vec2 offset() const;
		glm::vec2 size() const;

		glm::vec2 inverseSize() const;
		Rect asRect() const;

		//glm::vec2 normalize(const glm::vec2 & point) const;
		//glm::vec2 denormalize(const glm::vec2 & point) const;

		glm::mat4 createOrthoMatrix() const;

	private:
		glm::vec2 m_min, m_max;
	};

	class Rotation
	{
	public:
		Rotation(float sin, float cos);
		Rotation(float radians);

		glm::mat4 asMatrix() const;

	private:
		float m_sin, m_cos;
	};

	class MatrixTransform
	{
	public:
		static glm::vec2 project(glm::vec2 point, const glm::mat4 & matrix);
		static glm::vec2 unProject(glm::vec2 point, const glm::mat4 & inverseMatrix);

		MatrixTransform() : m_matrix(1.f), m_inverseMatrix(1.f) {}
		virtual ~MatrixTransform() = default;

		glm::mat4 getMatrix() const;
		glm::mat4 getInverseMatrix() const;

		glm::vec2 project(const glm::vec2 & point) const;
		glm::vec2 unProject(const glm::vec2 & point) const;

	protected:
		void update();
		virtual glm::mat4 calculateMatrix() const = 0;

	private:
		glm::mat4 m_matrix, m_inverseMatrix;
	};

	class Transform : public MatrixTransform
	{
	public:
		Transform();
		Transform(const glm::vec2 & pos, const Rotation & rot);

		void setIdentity();
		void set(const glm::vec2 & pos, const Rotation & rot);

	private:
		glm::mat4 calculateMatrix() const override final;
		glm::mat4 m_pos, m_rot;
	};

	class Camera : public MatrixTransform
	{
	public:
		static glm::vec2 project(glm::vec2 point, const glm::mat4 & matrix, const Geom::Bounds & viewport);
		static glm::vec2 unProject(glm::vec2 point, const glm::mat4 & inverseMatrix, const Geom::Bounds & viewport);

		virtual Rect getVisibleArea() const;

		glm::vec2 project(glm::vec2 point, const Geom::Bounds & viewport) const;
		glm::vec2 unProject(glm::vec2 point, const Geom::Bounds & viewport) const;
	};

	class TransformCamera : public Camera
	{
	public:
		TransformCamera();

		void setTransform(const Transform & transform);
		const Transform & getTransform() const;

	private:
		glm::mat4 calculateMatrix() const override final;
		Transform m_transform;
	};

	/*
		Scaling interface and implementations
	*/

	class Scaling
	{
	public:
		virtual Geom::Bounds getBounds(const glm::vec2 & viewportSize) const = 0;
		glm::mat4 createOrthoMatrix(const glm::vec2 & viewportSize) const;
		//glm::vec2 scalePoint(const glm::vec2 & point, const glm::vec2 & viewportSize, bool normalizedOutput) const;
		//glm::vec2 unscalePoint(const glm::vec2 & point, const glm::vec2 & viewportSize, bool normalizedInput) const;
	};

	class SizedScaling : public Scaling
	{
	public:
		SizedScaling(const glm::vec2 & targetSize) :
			m_targetSize(targetSize)
		{

		}

		glm::vec2 getTargetSize() const { return m_targetSize; }
		glm::vec2 setTargetSize(const glm::vec2 & size) { m_targetSize = size; }

	protected:
		glm::vec2 m_targetSize;
	};

	class StretchScaling : public SizedScaling
	{
	public:
		StretchScaling(const glm::vec2 & targetSize);
		Geom::Bounds getBounds(const glm::vec2 & viewportSize) const override;
	};

	class FitScaling : public SizedScaling
	{
	public:
		FitScaling(const glm::vec2 & targetSize);
		Geom::Bounds getBounds(const glm::vec2 & viewportSize) const override;
	};

	class ScaledCamera : private Camera
	{
	public:
		ScaledCamera(const glm::vec2 & viewportSize, const std::shared_ptr<Scaling> & scaling);

		void setViewportSize(const glm::vec2 & size);
		void setScaling(const std::shared_ptr<Scaling> & scaling);

	private:
		glm::mat4 calculateMatrix() const override final;

		glm::vec2 m_viewportSize;
		std::shared_ptr<Scaling> m_scaling;
	};
}

Geom::Rect operator*(const glm::mat4 & matrix, const Geom::Rect & rect);