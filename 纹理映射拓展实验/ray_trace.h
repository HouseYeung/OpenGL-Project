#pragma once
#include "lineTriangle3DIntersection.h"
#include "direct_lighting.h"
#include <atlconv.h>
#include <gdiplus.h>
#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;
#define M_PI 3.141592653589793
#define CUR_INFINITY 1e8
#define MAX_RAY_DEPTH 2
void samplepoint_sonprojectionplan(float left_x, float right_x, float bottom_y, float up_y, float
	nearplane_distance, float eye_z, std::map<my_3D_point_coord*, my_draw_color*>&
	render_vertices, unsigned& width, unsigned& height)
{
	//��ͶӰ������в����ܶ�����
	float x_delt = (right_x - left_x) / (ceil(right_x) - floor(left_x));
	float y_delt = (up_y - bottom_y) / (ceil(up_y) - floor(bottom_y));
	float z_val = eye_z - nearplane_distance - 1;
	width = 0;
	height = 0;
	bool counted = false;
	for (float x_iter = left_x; x_iter <= right_x; x_iter += x_delt)
	{
		width++;
		for (float y_iter = bottom_y; y_iter <= up_y; y_iter += y_delt)
		{
			my_3D_point_coord* tempPoint_ptr = new my_3D_point_coord(x_iter, y_iter,
				z_val);
			my_draw_color* tempColor_ptr = new my_draw_color{ 0, 0, 0 };
			render_vertices.insert(pair<my_3D_point_coord*,
				my_draw_color*>(tempPoint_ptr, tempColor_ptr));
			if (counted == false) height++;
		}
		counted = true;
	}
}
//�����ľ���
float distance(my_3D_point_coord p1, my_3D_point_coord p2)
{
	float dis = sqrt(pow((p1.x - p2.x), 2) + pow((p1.y - p2.y), 2) + pow((p1.z - p2.z), 2));
	return dis;
}
//��������Ƭ��һ���������ɫ
my_draw_color calculate_texture_color(my_3D_point_coord input_vertex, my_3D_point_coord
	p1, my_3D_point_coord p2, my_3D_point_coord p3,
	my_2D_Texture_coord p1TextCoord, my_2D_Texture_coord p2TextCoord,
	my_2D_Texture_coord p3TextCoord, int nearest_model_index)
{
	//��ȡ����ͼƬ
	wstring infilename;
	switch (nearest_model_index)
	{
	case 0:
	{
		infilename = L"��ɫ.bmp";
		break;
	}
	case 1:
	{
		infilename = L"��ɫ.bmp";
		break;
	}
	case 2:
	{
		infilename = L"��ɫ.bmp";
		break;
	}
	}
	//���������ظ������⣬����������0-1֮��
	if (p1TextCoord.u > 1.0) p1TextCoord.u--;
	else if (p1TextCoord.u < 0.0) p1TextCoord.u++;
	if (p1TextCoord.v > 1.0) p1TextCoord.v--;
	else if (p1TextCoord.v < 0.0) p1TextCoord.v++;
	if (p2TextCoord.u > 1.0) p2TextCoord.u--;
	else if (p2TextCoord.u < 0.0) p2TextCoord.u++;
	if (p2TextCoord.v > 1.0) p2TextCoord.v--;
	else if (p2TextCoord.v < 0.0) p2TextCoord.v++;
	if (p3TextCoord.u > 1.0) p3TextCoord.u--;
	else if (p3TextCoord.u < 0.0) p3TextCoord.u++;
	if (p3TextCoord.v > 1.0) p3TextCoord.v--;
	else if (p3TextCoord.v < 0.0) p3TextCoord.v++;
	//�������ľ���
	float d1 = distance(input_vertex, p1);
	float d2 = distance(input_vertex, p2);
	float d3 = distance(input_vertex, p3);
	//ϵ��
	float sum = d1 + d2 + d3;
	float k1 = d1 / sum;
	float k2 = d2 / sum;
	float k3 = d3 / sum;
	//�󽻵����������
	my_2D_Texture_coord input_vertex_coord;
	input_vertex_coord.u = k1 * p1TextCoord.u + k2 * p2TextCoord.u + k3 * p3TextCoord.u;
	input_vertex_coord.v = k1 * p1TextCoord.v + k2 * p2TextCoord.v + k3 * p3TextCoord.v;
	//ȡ���õ��������ɫ
	GdiplusStartupInput gdiplusstartupinput;
	ULONG_PTR gdiplustoken;
	GdiplusStartup(&gdiplustoken, &gdiplusstartupinput, NULL);
	Bitmap* bmp = new Bitmap(infilename.c_str());
	UINT height = bmp->GetHeight();
	UINT width = bmp->GetWidth();
	//�󽻵�������±�
	int input_vertex_index_u = floor(width * input_vertex_coord.u + 0.5);
	int input_vertex_index_v = floor(height * input_vertex_coord.v + 0.5);
	Color color;
	my_draw_color p1color, p2color, p3color, input_vertex_color;
	bmp->GetPixel(input_vertex_index_u, input_vertex_index_v, &color);
	input_vertex_color.r = (float)color.GetRed() / 255;
	input_vertex_color.g = (float)color.GetGreen() / 255;
	input_vertex_color.b = (float)color.GetBlue() / 255;
	delete bmp;
	GdiplusShutdown(gdiplustoken);
	return input_vertex_color;
}
//������������
bool get_refract_dir_my(const my_3Dvector& inpuray_dir, my_3Dvector& nhit, float ni_over_nt,
	my_3Dvector& refracted_dir)//
{
	my_3Dvector uv(inpuray_dir.dx, inpuray_dir.dy, inpuray_dir.dz);
	uv.normalized();
	float dt = uv.dot(nhit);
	float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
	if (discriminant > 0)
	{
		refracted_dir = (uv - nhit * dt) * ni_over_nt - nhit * sqrt(discriminant);
		return true;
	}
	else
		return false;
}
//���߸����㷨
my_draw_color one_ray_trace_my(my_3D_point_coord rayorig, my_3Dvector raydir, const
	std::vector<my_triangle_3DModel>& all_models, const int& depth,
	const my_3D_point_coord& eye_position, const my_3D_point_coord& light_position, float
	light_rgb_ambient[], float light_rgb_diffuse_specular[])//���ڽ���ֱ����ɫ����
{
	//�����ݹ����������000
	if (depth > MAX_RAY_DEPTH)
		return my_draw_color{ 0, 0, 0 };
	//����ģ�ͣ����ߵĽ��㣩���������������������󽻡���һ��Ҫע��������������ϵ�����
	float nearest_t = INFINITY; //���߲��������е�t
	my_3Dvector nearestTrangleNormal;
	const my_triangle_3DModel* nearestModel = NULL;
	my_3D_line curRay(rayorig.x, rayorig.y, rayorig.z, raydir.dx, raydir.dy, raydir.dz);
	my_3D_point_coord point1, point2, point3;
	my_2D_Texture_coord textCoord1, textCoord2, textCoord3;
	int nearest_model_index;
	for (unsigned model_index = 0; model_index < all_models.size(); model_index++)
	{
		for (unsigned int tri_index = 0; tri_index < all_models[model_index].faceSets.size();
			tri_index++)
		{
			//����ʵ����ȡ����������
			int firstPointIndex =
				all_models[model_index].faceSets[tri_index].first_point_index;//ȡ����������
			int secondPointIndex =
				all_models[model_index].faceSets[tri_index].second_point_index;
			int thirdPointIndex =
				all_models[model_index].faceSets[tri_index].third_point_index;
			int firstTextIndex =
				all_models[model_index].faceSets[tri_index].first_point_texture_index;//����������
			int secondTextIndex =
				all_models[model_index].faceSets[tri_index].second_point_texture_index;
			int thirdTextIndex =
				all_models[model_index].faceSets[tri_index].third_point_texture_index;
			my_3D_point_coord p1 = all_models[model_index].pointSets[firstPointIndex];//��һ������
			my_3D_point_coord p2 = all_models[model_index].pointSets[secondPointIndex];
			//�ڶ�������
			my_3D_point_coord p3 = all_models[model_index].pointSets[thirdPointIndex]; //����������
			my_2D_Texture_coord p1TextCoord =
				all_models[model_index].pointTextureSets[firstTextIndex];//��һ�������������
			my_2D_Texture_coord p2TextCoord =
				all_models[model_index].pointTextureSets[secondTextIndex];//�ڶ�������������
			my_2D_Texture_coord p3TextCoord =
				all_models[model_index].pointTextureSets[thirdTextIndex];//�����������������
			my_3D_triangle curTriangle;
			curTriangle.first_point = p1;
			curTriangle.second_point = p2;
			curTriangle.third_point = p3;
			curTriangle.isFilled = true;
			IntersectionBetweenLineAndTriangle newIntTest(curRay, curTriangle);
			//�����н��㣬��Ҫ�����ǳ�����newIntTest.GetLineParameter() > 1e-3 
			if (newIntTest.Find() && newIntTest.GetLineParameter() > 0.002f &&
				(newIntTest.GetLineParameter() < nearest_t))
			{
				nearestModel = &all_models[model_index];
				nearestTrangleNormal = newIntTest.GetHitPointNormal();
				nearest_t = newIntTest.GetLineParameter();
				//��ֵ
				point1 = p1;
				point2 = p2;
				point3 = p3;
				textCoord1 = p1TextCoord;
				textCoord2 = p2TextCoord;
				textCoord3 = p3TextCoord;
				nearest_model_index = model_index;
			}
		}
	}
	if (!nearestModel)
		return depth == 0 ? my_draw_color{ 1, 1, 1 } : my_draw_color{ 0, 0, 0 };
	my_3Dvector added_valVec = raydir * nearest_t;
	my_3D_point_coord phit = rayorig.add(added_valVec.dx, added_valVec.dy,
		added_valVec.dz); // ��ý���
	my_3Dvector nhit = nearestTrangleNormal; // ��ý��㴦�ķ���
	nhit.normalized();
	my_draw_color phit_textCoord_color = calculate_texture_color(phit, point1, point2, point3,
		textCoord1, textCoord2, textCoord3, nearest_model_index);
	//��Blinn-Phong Reflection Model���㽻����ɫ ����ֻҪ�л��о���һ���Ƿ��ܱ���Դ���ǵ���������ֱ�䡢���仹������
	float ambient_rgb_reflection[3] = {
	nearestModel->material_ambient_rgb_reflection[0], nearestModel->material_ambient_rgb_reflection[1], nearestModel->material_ambient_rgb_reflection[2] };
	float diffuse_rgb_reflection[3] = { nearestModel->material_diffuse_rgb_reflection[0],
		nearestModel->material_diffuse_rgb_reflection[1],
		nearestModel->material_diffuse_rgb_reflection[2] };
	float specular_rgb_reflection[3] = {
		nearestModel->material_specular_rgb_reflection[0], nearestModel->material_specular_rgb_reflection[1], nearestModel->material_specular_rgb_reflection[2] };
	my_draw_color surface_directColor =
		calculate_direct_light_on_one_vertex_usingBPRM(phit, nhit, eye_position, light_position,
			light_rgb_ambient, ambient_rgb_reflection,
			light_rgb_diffuse_specular, diffuse_rgb_reflection,
			specular_rgb_reflection, nearestModel->ns);
	//������ջ��
	surface_directColor = surface_directColor * 0.5;
	surface_directColor.r += (phit_textCoord_color.r * 0.5);
	surface_directColor.g += (phit_textCoord_color.g * 0.5);
	surface_directColor.b += (phit_textCoord_color.b * 0.5);
	//�����ڲ��㣬˵��������ģ���ڲ��߶����򽻵��ķ�����Ŀǰ�������ֻ���������������������ֹ
	bool inside = false;
	if (raydir.dot(nhit) > 0)
	{
		inside = true;
		nhit = nhit * -1;
	}
	//���㷴��⹱�׵���ɫ
	my_draw_color reflectionColor = { 0, 0, 0 };
	if (nearestModel->reflection > 0 && inside == false)
	{
		my_3Dvector refldir = raydir - nhit * 2 * raydir.dot(nhit);//���㷴��ⷽ��
		refldir.normalized();
		reflectionColor = one_ray_trace_my(phit, refldir, all_models, depth + 1, eye_position,
			light_position, light_rgb_ambient, light_rgb_diffuse_specular); //+ nhit*bias
		reflectionColor = reflectionColor * 0.5;//0.5 ����ϵ��
	}
	//��������⹱�׵���ɫ
	my_draw_color refractionColor = { 0, 0, 0 };
	if (nearestModel->transparency > 0)
	{
		my_3Dvector refrdir = raydir;
		float ni_over_nt = inside ? nearestModel->transparency / 1.00029 : 1.00029 /
			nearestModel->transparency; //1.00029 Ϊ������������
		bool refrected = get_refract_dir_my(raydir, nhit, ni_over_nt, refrdir); //��������⹱�׵���ɫ
		if (refrected)
		{
			refrdir.normalized();
			refractionColor = one_ray_trace_my(phit, refrdir, all_models, depth + 1,
				eye_position, light_position, light_rgb_ambient, light_rgb_diffuse_specular); //- nhit*bias
			refractionColor = refractionColor * 0.5; //0.5����ϵ��
		}
	}
	return surface_directColor + reflectionColor + refractionColor;
}
	//������Ȼ�������
	struct my_map
	{
		float shadow = 0;//��ʼ����Ϊ��ɫ����������Ӱ֮�У�ȡֵΪ0��1֮��ĸ�����
		float t = 999999;//������¼�������ģ���ϵĵ�ľ��룬��ʼֵΪ���
		my_3D_point_coord models_crossed_point;//͸��������Ĺ�����ģ���ཻ�ĵ�
	}my_map_point[800][800];
	my_3Dvector facenormal;
	my_3D_point_coord CalPlaneLineIntersectPoint(my_3Dvector planeVector,
		my_3D_point_coord planePoint, my_3Dvector lineVector, my_3D_point_coord
		linePoint)
	 {
		my_3D_point_coord crosspoint;
		float vp1, vp2, vp3, n1, n2, n3, v1, v2, v3, m1, m2, m3, t, vpt;
		vp1 = planeVector.dx;
		vp2 = planeVector.dy;
		vp3 = planeVector.dz;
		n1 = planePoint.x;
		n2 = planePoint.y;
		n3 = planePoint.z;
		v1 = lineVector.dx;
		v2 = lineVector.dy;
		v3 = lineVector.dz;
		m1 = linePoint.x;
		m2 = linePoint.y;
		m3 = linePoint.z;
		vpt = v1 * vp1 + v2 * vp2 + v3 * vp3;
		//�����ж�ֱ���Ƿ���ƽ��ƽ��
		if (vpt == 0)
		{
			return { 0,0,0 };
		}
		else
		{
			t = ((n1 - m1) * vp1 + (n2 - m2) * vp2 + (n3 - m3) * vp3) / vpt;
			crosspoint.x = m1 + v1 * t;
			crosspoint.y = m2 + v2 * t;
			crosspoint.z = m3 + v3 * t;
		}
		return crosspoint;
	}

	/***************************************
	* ������Ӱ
	* left_x, right_x, bottom_y, up_yΪͶӰ�棨�ӿڣ������ĸ�����ı߽�ֵ
	* rayorig��raydir�ֱ�Ϊ���߷�����λ�úͳ���
	* all_models �����е�������άͼ��
	* eye_positionΪ�ӵ�λ�� light_position��Դλ��
	* ȫ�ֱ���my_map_pointΪһ����Ȼ�����󣨶�ά���飩������Ϊmy_map
	* ���ص�ǰ�����Ӱֵ
	***************************************/
	float one_ray_trace_shadow(float left_x, float right_x, float bottom_y, float up_y,
		my_3D_point_coord rayorig, my_3Dvector raydir,
		const vector<my_triangle_3DModel>& all_models,
		const my_3D_point_coord& light_position)
	{
		//����ģ�ͣ����ߵĽ��㣩���������������������󽻡���һ��Ҫע��������������ϵ�����
		float nearest_t = INFINITY; //���߲��������е�t
		my_3Dvector nearestTrangleNormal;
		const my_triangle_3DModel* nearestModel = NULL;
		my_3D_line curRay(rayorig.x, rayorig.y, rayorig.z, raydir.dx, raydir.dy, raydir.dz);
		for (unsigned model_index = 0; model_index < all_models.size(); model_index++)
		{
			for (unsigned int tri_index = 0; tri_index < all_models[model_index].faceSets.size();
				tri_index++)
			{
				//����ʵ����ȡ����������
				int firstPointIndex =
					all_models[model_index].faceSets[tri_index].first_point_index;
				int secondPointIndex =
					all_models[model_index].faceSets[tri_index].second_point_index;
				int thirdPointIndex =
					all_models[model_index].faceSets[tri_index].third_point_index;
				my_3D_point_coord p1 =
					all_models[model_index].pointSets[firstPointIndex];//��һ������
				my_3D_point_coord p2 =
					all_models[model_index].pointSets[secondPointIndex]; //�ڶ�������
				my_3D_point_coord p3 =
					all_models[model_index].pointSets[thirdPointIndex]; //����������
				my_3D_triangle curTriangle = { p1, p2, p3 };
				IntersectionBetweenLineAndTriangle newIntTest(curRay, curTriangle);
				//�����н��㣬��Ҫ�����ǳ����㸽������0.002��Χ�ڵĵ�
				if (newIntTest.Find() && newIntTest.GetLineParameter() > 0.002f &&
					(newIntTest.GetLineParameter() < nearest_t))
				{
					nearestModel = &all_models[model_index];
					nearestTrangleNormal = newIntTest.GetHitPointNormal();
					nearest_t = newIntTest.GetLineParameter();
				}
			}
		}
		//��ǰ������ģ���޽������������ʾ���߻����˿ռ䱳��������Ӱ�����ذ�ɫ
		if (!nearestModel)
			return 1;
		my_3Dvector added_valVec = raydir * nearest_t;
		my_3D_point_coord phit = rayorig.add(added_valVec.dx, added_valVec.dy,
			added_valVec.dz); // ��ý���
		my_3Dvector nhit = nearestTrangleNormal; // ��ý��㴦�ķ���
		nhit.normalized();
		int i, j;
		float shadow_color = 0;
		my_3D_point_coord crosspoint;//����Ȼ�����Ľ���
		my_3Dvector light_to_point(light_position, phit);
		//��õ�����Ȼ�����Ľ���
		crosspoint = CalPlaneLineIntersectPoint(my_3Dvector(0, -1, 0),
			my_3D_point_coord(-left_x, 1000, -bottom_y), light_to_point, phit);
		//�жϽ����������ϵĴ���λ��
		if (crosspoint.x<left_x || crosspoint.x>right_x || crosspoint.z<bottom_y || crosspoint.z>up_y)//�ڻ����淶Χ֮�⣬����Դ�޷����ǵ����򣬷��غ�ɫ
		{
			return 0;
		}
		else//����Ȼ�������ʵ����
		{
			float x_delt = (right_x - left_x) / (ceil(right_x) - floor(left_x));//����������
			float y_delt = (up_y - bottom_y) / (ceil(up_y) - floor(bottom_y)); //����������
			float ii = (crosspoint.x - left_x) / x_delt;
			float jj = (crosspoint.z - bottom_y) / y_delt;
			int ii_up = ceil(ii);//i���Ͻ�
			int ii_down = floor(ii);//i���½�
			int jj_up = ceil(jj);
			int jj_down = floor(jj);
			//�պ���my_map_point���ҵ���Ӧ�ĵ�
			if (ii_down == ii_up && jj_down == jj_up)
			{
				i = ii_up, j = jj;
				if (distance(phit, my_map_point[i][j].models_crossed_point) <= 1)
				{
					shadow_color = my_map_point[i][j].shadow;
				}
				else
					shadow_color = 0;
			}
			else//�ҵ���Χ��ֵ��Χ��(���4��)�ĵ㣬����ƽ����Ӱֵ
			{
				int count = 0;
				if (distance(phit, my_map_point[ii_up][jj_up].models_crossed_point) < 5)
					shadow_color += my_map_point[ii_up][jj_up].shadow; count++;
				if (distance(phit, my_map_point[ii_up][jj_down].models_crossed_point) < 5)
					shadow_color += my_map_point[ii_up][jj_down].shadow; count++;
				if (distance(phit, my_map_point[ii_down][jj_down].models_crossed_point) < 5)
					shadow_color += my_map_point[ii_down][jj_down].shadow; count++;
				if (distance(phit, my_map_point[ii_down][jj_up].models_crossed_point) < 5)
					shadow_color += my_map_point[ii_down][jj_up].shadow; count++;
				shadow_color /= count;
			}
		}
		return shadow_color;
	}
	/***************************************
	* ������Ȼ������
	* ������Ҫ�����ӵ�����������ϵ��ĳ�����غϣ���ͶӰƽ����ĳ������ƽ��ƽ��
	* ���������ᴹֱ����������ƽ��
	* left_x, right_x, bottom_y, up_yΪͶӰ�棨�ӿڣ������ĸ�����ı߽�ֵ
	* light_position��Դλ��
	* all_models �����е�������άͼ��
	* ȫ�ֱ���my_map_pointΪһ����ά�����飬����Ϊmy_map
	***************************************/
	void build_shadow_map(
		float left_x, float right_x, float bottom_y, float up_y,
		my_3D_point_coord& light_position,
		const std::vector<my_triangle_3DModel>& all_models)
	{
		//�������֮��ļ�����ܶ�
		float x_delt = (right_x - left_x) / (ceil(right_x) - floor(left_x));
		float y_delt = (up_y - bottom_y) / (ceil(up_y) - floor(bottom_y));
		int i, j;//��Ϊmap���±�
		//������������������
		int max_i = ceil(abs(right_x - left_x) / x_delt);
		int max_j = ceil(abs(up_y - bottom_y) / y_delt);
		float t = 0;
		for (i = 0; i < max_i; i++)
		{
			for (j = 0; j < max_j; j++)
			{
				//��Ȼ������ϵ������
				my_3D_point_coord samplepoint(left_x + i * x_delt, 1000, bottom_y + j *
					y_delt);//������
				my_3D_point_coord lightposition(light_position.x, light_position.y,
					light_position.z);//��Դ����
				my_3Dvector lightvector(lightposition, samplepoint);//��������
				lightvector.normalized();
				my_3D_line light_line(lightposition, lightvector);//��������
				//����all_models�е�ÿ����άͼ�β���light_line�󽻣�ȷ�����Դ����Ľ��㣻���Ľ��������Ȼ������my_map_point[i][j]
				for (unsigned model_index = 0; model_index < all_models.size(); model_index++)
				{
					for (unsigned int tri_index = 0; tri_index <
						all_models[model_index].faceSets.size(); tri_index++)
					{
						//����ʵ����ȡ����������
						int firstPointIndex =
							all_models[model_index].faceSets[tri_index].first_point_index;//ȡ����������
						int secondPointIndex =
							all_models[model_index].faceSets[tri_index].second_point_index;
						int thirdPointIndex =
							all_models[model_index].faceSets[tri_index].third_point_index;
						my_3D_point_coord p1 =
							all_models[model_index].pointSets[firstPointIndex];//��һ������
						my_3D_point_coord p2 =
							all_models[model_index].pointSets[secondPointIndex]; //�ڶ�������
						my_3D_point_coord p3 =
							all_models[model_index].pointSets[thirdPointIndex]; //����������
						my_3D_triangle curTriangle = { p1, p2, p3 };
						IntersectionBetweenLineAndTriangle getcross(light_line, curTriangle);
						if (getcross.Find())
						{
							float t = getcross.GetLineParameter();
							if (my_map_point[i][j].t >= t)
							{
								my_3Dvector added_valVec = lightvector * t;
								my_3D_point_coord phit =
									light_position.add(added_valVec.dx, added_valVec.dy, added_valVec.dz); // ��ý���
								my_map_point[i][j].t = t;
								my_map_point[i][j].shadow = 1;//����㱻����
								my_map_point[i][j].models_crossed_point = phit;//�����������������ߺ�ģ���ཻ�ĵ�
							}
						}
					}
				}
				cout << i << endl;
			}
		}
}