#define GLUT_DISABLE_ATEXIT_HACK
#include "direct_lighting.h"
#include "ray_trace.h"
#include "GLUT.H"
#include <math.h> 
#include <atlconv.h>
#include <Windows.h>
int nearplane_width = 600; //�Ӿ�����
int nearplane_height = 400; //�Ӿ���߶�
int nearplane_distance = 600; //�Ӿ����ƽ�����ӵ����
int farplane_distance = nearplane_distance + 2000; //�Ӿ���Զƽ�����ӵ����
my_3D_point_coord eye_position = { 0,0,1000 }; //�ӵ�λ��
float theta = 0.1;//�ӵ�����ת
float theta00 = 0.1;
int numrotate[5] = { 0,0,0,0,0 };
int xuanze = 0;
float radius = 2400;
my_3Dvector v(eye_position.x, eye_position.y, eye_position.z);
std::map<my_3D_point_coord*, my_draw_color*> render_vertices;//������Ҫ���Ƶĵ��Լ������㡣
std::vector< my_triangle_3DModel> cur3DModel; //����������ģ��
int width = 0, height = 0;
my_3Dvector eyefacenormal = { 1,1,1 };//��Դ������ƽ�淨����
my_3D_point_coord light_position(2000.0, 2000.0, 2000.0); //��Դλ��
float light_rgb_ambient[] = { 0.1, 0.1, 0.1 }; //������
float light_rgb_diffuse_specular[] = { 0.8, 0.8, 0.8 };//������⡢����������
bool open_light = false;
bool rendered = false;
unsigned image_w, image_h;
//������ص���Ϣ
GLuint textName[10];
int xmove[5] = { 0,0,0,0,0 }, ymove[5] = { 0,0,0,0,0 }, zmove[5] = { 0,0,0,0,0 };
//�����ƶ�����
int xmove0 = 0, ymove0 = 0, zmove0 = 0;
//��ʼ������ģ��
//����ģ�ͣ����ù�Դ������ٶ��ӵ��� Z �ᣬͶӰ���� xoy ƽ��ƽ��
void init(void)
{
	//����Թ��ߵķ�����
	float material_ambient_rgb_reflection[] = { 0.2, 0.2, 0.2 };
	float material_specular_rgb_reflection[] = { 0.8,0.8,0.8 };
	float ns = 10; //�۹�ָ��
	//////////////////////////////////ģ�� 1/////////////////////////////////////////////////
	ObjLoader cur_objLoader0 = ObjLoader("����0.obj");
	float obj0_material_diffuse_rgb_reflection[] = { 0.7, 0.7, 0.7 };
	cur_objLoader0.my_3DModel.modify_color_configuration(9, 1,
		material_ambient_rgb_reflection, obj0_material_diffuse_rgb_reflection,
		material_specular_rgb_reflection, ns);
	cur3DModel.push_back(cur_objLoader0.my_3DModel);
	glGenTextures(1, &textName[0]);
	wchar_t file_path1[] = L"mutou01.bmp";
	HBITMAP hBMP1 = (HBITMAP)LoadImage(NULL, file_path1, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);;
	BITMAP BMP;
	GetObject(hBMP1, sizeof(BMP), &BMP);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP.bmWidth, BMP.bmHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP.bmBits);
	DeleteObject(hBMP1);
	/*//////////////////////////////////ģ�� 2/////////////////////////////////////////////////
	ObjLoader cur_objLoader1 = ObjLoader("����0.obj");
	float obj1_material_diffuse_rgb_reflection[] = { 0.7, 0.7, 0.7 };
	cur_objLoader1.my_3DModel.modify_color_configuration(1000, 1,
		material_ambient_rgb_reflection, obj1_material_diffuse_rgb_reflection,
		material_specular_rgb_reflection, ns);
	cur3DModel.push_back(cur_objLoader1.my_3DModel);
	glGenTextures(1, &textName[1]);
	wchar_t file_path2[] = L"gang.bmp";
	HBITMAP hBMP2 = (HBITMAP)LoadImage(NULL, file_path2, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION | LR_LOADFROMFILE);;
	BITMAP BMP2;
	GetObject(hBMP2, sizeof(BMP2), &BMP2);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[1]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP2.bmWidth, BMP2.bmHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP2.bmBits);
	DeleteObject(hBMP2);
	/////////////////////////////////////ģ�� 3/////////////////////////////////////////////////
	ObjLoader cur_objLoader2 = ObjLoader("����02.obj");
	float obj2_material_diffuse_rgb_reflection[] = { 0.7, 0.7, 0.7 };
	cur_objLoader2.my_3DModel.modify_color_configuration(1000, 1,
		material_ambient_rgb_reflection, obj2_material_diffuse_rgb_reflection,
		material_specular_rgb_reflection, ns);
	cur3DModel.push_back(cur_objLoader2.my_3DModel);
	glGenTextures(1, &textName[2]);
	wchar_t file_path3[] = L"wood.bmp";
	HBITMAP hBMP3 = (HBITMAP)LoadImage(NULL, file_path3, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);;
	BITMAP BMP3;
	GetObject(hBMP3, sizeof(BMP3), &BMP3);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[2]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP3.bmWidth, BMP3.bmHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP3.bmBits);
	DeleteObject(hBMP3);
	/////////////////////////////////////ģ�� 4/////////////////////////////////////////////////
	ObjLoader cur_objLoader3 = ObjLoader("��0.obj");
	float obj3_material_diffuse_rgb_reflection[] = { 0.7, 0.7, 0.7 };
	cur_objLoader3.my_3DModel.modify_color_configuration(1000, 1,
		material_ambient_rgb_reflection, obj3_material_diffuse_rgb_reflection,
		material_specular_rgb_reflection, ns);
	cur3DModel.push_back(cur_objLoader3.my_3DModel);
	glGenTextures(1, &textName[3]);
	wchar_t file_path4[] = L"01.bmp";
	HBITMAP hBMP4 = (HBITMAP)LoadImage(NULL, file_path4, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);;
	BITMAP BMP4;
	GetObject(hBMP4, sizeof(BMP4), &BMP4);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	glBindTexture(GL_TEXTURE_2D, textName[3]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP4.bmWidth, BMP4.bmHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP4.bmBits);
	DeleteObject(hBMP4);
	/////////////////////////////////////ģ�� 5/////////////////////////////////////////////////
	ObjLoader cur_objLoader4 = ObjLoader("��0.obj");
	float obj4_material_diffuse_rgb_reflection[] = { 0.7, 0.7, 0.7 };
	cur_objLoader4.my_3DModel.modify_color_configuration(1000, 1,
		material_ambient_rgb_reflection, obj4_material_diffuse_rgb_reflection,
		material_specular_rgb_reflection, ns);
	cur3DModel.push_back(cur_objLoader4.my_3DModel);
	glGenTextures(1, &textName[4]);
	wchar_t file_path5[] = L"buxiugang.bmp";
	HBITMAP hBMP5 = (HBITMAP)LoadImage(NULL, file_path5, IMAGE_BITMAP, 0, 0,
		LR_CREATEDIBSECTION | LR_LOADFROMFILE);;
	BITMAP BMP5;
	GetObject(hBMP5, sizeof(BMP5), &BMP5);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 5);
	glBindTexture(GL_TEXTURE_2D, textName[4]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, 3, BMP5.bmWidth, BMP5.bmHeight, 0,
		GL_BGR_EXT, GL_UNSIGNED_BYTE, BMP5.bmBits);
	DeleteObject(hBMP5);*/
}
//��������
void display(void)
{
	glClearColor(1.f, 1.f, 1.f, 0.f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (!rendered)
	{
		//ֱ�ӹ����
		glShadeModel(GL_SMOOTH);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
		//��������
		for (unsigned int model_index = 0; model_index < cur3DModel.size(); model_index++)
		{
			glBindTexture(GL_TEXTURE_2D, textName[model_index]);
			for (unsigned int i = 0; i < cur3DModel[model_index].faceSets.size(); i++)
			{
				//����ʵ����ȡ���������� 
				my_triangle_indices curTriangle = cur3DModel[model_index].faceSets[i];
				my_3D_point_coord p1 =
					cur3DModel[model_index].pointSets[curTriangle.first_point_index];//��һ������
				my_3D_point_coord p2 =
					cur3DModel[model_index].pointSets[curTriangle.second_point_index]; //�ڶ�������
				my_3D_point_coord p3 =
					cur3DModel[model_index].pointSets[curTriangle.third_point_index]; //����������
				my_2D_Texture_coord p1TextCoord =
					cur3DModel[model_index].pointTextureSets[curTriangle.first_point_texture_index];//�� һ �� ��������
				my_2D_Texture_coord p2TextCoord =
					cur3DModel[model_index].pointTextureSets[curTriangle.second_point_texture_index];//�� �� ����������
				my_2D_Texture_coord p3TextCoord =
					cur3DModel[model_index].pointTextureSets[curTriangle.third_point_texture_index];//�� �� �� ��������
				my_3Dvector p1Normal =
					cur3DModel[model_index].pointNormalSets[curTriangle.first_point_normal_index];//�� һ �� ���㷨��
				my_3Dvector p2Normal =
					cur3DModel[model_index].pointNormalSets[curTriangle.second_point_normal_index];//�� �� �����㷨��
				my_3Dvector p3Normal =
					cur3DModel[model_index].pointNormalSets[curTriangle.third_point_normal_index];//�� �� �� ���㷨��
				my_draw_color p1color, p2color, p3color;
				p1color.r = p1color.g = p1color.b = 0;
				p2color.r = p2color.g = p2color.b = 0;
				p3color.r = p3color.g = p3color.b = 0;

				p1.x += xmove0;
				p2.x += xmove0;
				p3.x += xmove0;

				p1.y += ymove0;
				p2.y += ymove0;
				p3.y += ymove0;

				p1.z += zmove0;
				p2.z += zmove0;
				p3.z += zmove0;


				
					p1.x += xmove[model_index];
					p2.x += xmove[model_index];
					p3.x += xmove[model_index];

					p1.y += ymove[model_index];
					p2.y += ymove[model_index];
					p3.y += ymove[model_index];

					p1.z += zmove[model_index];
					p2.z += zmove[model_index];
					p3.z += zmove[model_index];

					for (int j = 0; j < numrotate[model_index]; j++)
					{
						p1.x = p1.x * cosf(-theta00) + p1.z * sinf(-theta00);
						p1.z = p1.z * cosf(-theta00) - p1.x * sinf(-theta00);

						p2.x = p2.x * cosf(-theta00) + p2.z * sinf(-theta00);
						p2.z = p2.z * cosf(-theta00) - p2.x * sinf(-theta00);

						p3.x = p3.x * cosf(-theta00) + p3.z * sinf(-theta00);
						p3.z = p3.z * cosf(-theta00) - p3.x * sinf(-theta00);
					}
				


				if (open_light)
				{
					//�� Blinn-Phong Reflection Model ����ÿһ�㵽��Դλ�õ�����
					p1color = calculate_direct_light_on_one_vertex_usingBPRM(p1,
						p1Normal, eye_position, light_position, light_rgb_ambient,
						cur3DModel[model_index].material_ambient_rgb_reflection,
						light_rgb_diffuse_specular,
						cur3DModel[model_index].material_diffuse_rgb_reflection,
						cur3DModel[model_index].material_specular_rgb_reflection, cur3DModel[model_index].ns);
					p2color = calculate_direct_light_on_one_vertex_usingBPRM(p2,
						p2Normal, eye_position, light_position, light_rgb_ambient,
						cur3DModel[model_index].material_ambient_rgb_reflection,
						light_rgb_diffuse_specular,
						cur3DModel[model_index].material_diffuse_rgb_reflection,
						cur3DModel[model_index].material_specular_rgb_reflection, cur3DModel[model_index].ns);
					p3color = calculate_direct_light_on_one_vertex_usingBPRM(p3,
						p3Normal, eye_position, light_position, light_rgb_ambient,
						cur3DModel[model_index].material_ambient_rgb_reflection,
						light_rgb_diffuse_specular,
						cur3DModel[model_index].material_diffuse_rgb_reflection,
						cur3DModel[model_index].material_specular_rgb_reflection, cur3DModel[model_index].ns);
				}
				glBegin(GL_TRIANGLES);//��ʼ����
				glTexCoord2f(p1TextCoord.u, p1TextCoord.v);
				glColor3f(p1color.r, p1color.g, p1color.b);
				glVertex3f(p1.x, p1.y, p1.z);
				glTexCoord2f(p2TextCoord.u, p2TextCoord.v);
				glColor3f(p2color.r, p2color.g, p2color.b);
				glVertex3f(p2.x, p2.y, p2.z);
				glTexCoord2f(p3TextCoord.u, p3TextCoord.v);
				glColor3f(p3color.r, p3color.g, p3color.b);
				glVertex3f(p3.x, p3.y, p3.z);
				glEnd();
			}
		}
	}
	else
	{
		glBegin(GL_POINTS);//��ʼ����
		for (std::map<my_3D_point_coord*, my_draw_color*>::iterator piter =
			render_vertices.begin(); piter != render_vertices.end(); piter++)
		{
			glColor3f(piter->second->r, piter->second->g, piter->second->b);
			glVertex3f(piter->first->x, piter->first->y, piter->first->z);
		}
		glEnd();
	}
	glutSwapBuffers();
}
///////////////////////////////////////////////////////////////////////////////////////////
//���̽����¼�
void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'O':
	case 'o': {
		zmove0 -= 10;
		glutPostRedisplay();
		break;
	}
	case 'i':
	case 'I': {
		zmove0 += 10;
		glutPostRedisplay();
		break;
	}
	case 'b'://
	case 'B':
	{
		xmove0 -= 10;
		glutPostRedisplay();
		break;
	}
	case 'm'://
	case 'M':
	{
		xmove0 += 10;
		glutPostRedisplay();
		break;
	}
	case 'n'://
	case 'N':
	{
		ymove0 -= 10;
		glutPostRedisplay();
		break;
	}
	case 'h'://
	case 'H':
	{
		ymove0 += 10;
		glutPostRedisplay();
		break;
	}
	case 'a'://
	case 'A':
	{
		xmove[xuanze] -= 10;
		glutPostRedisplay();
		break;
	}
	case 'd'://
	case 'D':
	{
		xmove[xuanze] += 10;
		glutPostRedisplay();
		break;
	}
	case 's'://
	case 'S':
	{
		ymove[xuanze] -= 10;
		glutPostRedisplay();
		break;
	}
	case 'w'://
	case 'W':
	{
		ymove[xuanze] += 10;
		glutPostRedisplay();
		break;
	}
	case 'f'://
	case 'F':
	{
		zmove[xuanze] -= 10;
		glutPostRedisplay();
		break;
	}
	case 'g'://
	case 'G':
	{
		zmove[xuanze] += 10;
		glutPostRedisplay();
		break;
	}
	case 'p'://
	case 'P':
	{
		for (int i = 0; i <= 5; i++)
		{
			zmove[i] = 0;
			ymove[i] = 0;
			xmove[i] = 0;
		}
		glutPostRedisplay();
		break;
	}
	case 'q'://
	case 'Q':
	{
		numrotate[xuanze]++;
		glutPostRedisplay();
		break;
	}
	case 'e'://
	case 'E':
	{
		numrotate[xuanze]--;
		glutPostRedisplay();
		break;
	}
	case'1':
	{
		light_rgb_ambient[0] += 0.3;
		light_rgb_ambient[1] += 0.3;
		light_rgb_ambient[2] += 0.3;
		glutPostRedisplay();
		break;
	}
	case'2':
	{
		light_rgb_ambient[0] -= 0.3;
		light_rgb_ambient[1] -= 0.3;
		light_rgb_ambient[2] -= 0.3; light_position;
		glutPostRedisplay();
		break;
	}
	case'3':
	{
		light_position.x += 300;
		light_position.y += 30;
		light_position.z += 300;
		cout << "��Դλ��" << light_position.x << "," << light_position.y << "," << light_position.z << endl;
		glutPostRedisplay();
		break;
	}
	case'4':
	{
		light_position.x -= 300;
		light_position.y -= 30;
		light_position.z -= 300;
		cout << "��Դλ��" << light_position.x << "," << light_position.y << "," << light_position.z << endl;
		glutPostRedisplay();
		break;
	}
	case'9':
	{
		xuanze++;
		cout << "Ŀǰ����Ϊ" << xuanze << endl;
		break;
	}
	case'0':
	{
		xuanze--;
		cout << "Ŀǰ����Ϊ" << xuanze << endl;
		break;
	}

	case 'z'://�� Zbuffer ��Ȳ���
	case 'Z':
	{
		glEnable(GL_DEPTH_TEST); // ����Ȼ������
		glDepthFunc(GL_LESS); // �ж��ڵ���ϵʱ�����ӵ���������ڵ����ӵ�Զ������
		glutPostRedisplay();
		break;
	}
	case 'c'://�ر� Zbuffer ��Ȳ���
	case 'C':
	{
		glDisable(GL_DEPTH_TEST); // �ر���Ȼ������
		glutPostRedisplay();
		break;
	}
	case 'l'://�򿪵ƹ�
	case 'L':
	{
		open_light = true;
		glutPostRedisplay();
		break;
	}
	case 'k'://�򿪵ƹ�
	case 'K':
	{
		open_light = false;
		glutPostRedisplay();
		break;
	}
	case 't':
	case 'T':
	{
		//�� OpenGL ֧������
		glEnable(GL_TEXTURE_2D);
		glutPostRedisplay();
		break;
	}
	case 'u':
	case 'U':
	{
		//�� OpenGL ��֧������
		glDisable(GL_TEXTURE_2D);
		glutPostRedisplay();
		break;
	}
	case 'r'://���߸���
	case 'R':
	{
		cout << "������" << endl;
		int i = 1;
		samplepoint_sonprojectionplan(-0.5 * nearplane_width, 0.5 * nearplane_width,
			-0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			nearplane_distance, eye_position.z, render_vertices, image_w, image_h);
		build_shadow_map(-0.5 * nearplane_width, 0.5 * nearplane_width,
			-0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, light_position,
			cur3DModel);
		for (std::map<my_3D_point_coord*, my_draw_color*>::iterator piter =
			render_vertices.begin(); piter != render_vertices.end(); piter++)
		{
			my_3Dvector raydir(eye_position, *(piter->first));
			raydir.normalized();
			my_draw_color newColor2 = one_ray_trace_my(eye_position, raydir, cur3DModel,
				0, eye_position, light_position, light_rgb_ambient, light_rgb_diffuse_specular);
			float ShawdowColor = one_ray_trace_shadow(-nearplane_width * 0.5,
				nearplane_width * 0.5, -nearplane_height * 0.5, nearplane_height * 0.5, eye_position, raydir,
				cur3DModel, light_position);
			my_draw_color newColor = { ShawdowColor, ShawdowColor, ShawdowColor };

			my_draw_color newColor3 = { 0, 0, 0 };
			newColor3.r = newColor.r * newColor2.r * 0.2 + newColor2.r * 0.8;
			newColor3.g = newColor.g * newColor2.g * 0.2 + newColor2.g * 0.8;
			newColor3.b = newColor.b * newColor2.b * 0.2 + newColor2.b * 0.8;
			*(piter->second) = newColor3;
			i++;
			cout << i << endl;
		}
		cout << "ү�����ˣ�" << endl;
		rendered = true;
		glutPostRedisplay();
		break;
	}
	case 'j':
	case 'J':
	{
		samplepoint_sonprojectionplan(-0.5 * nearplane_width, 0.5 * nearplane_width,
			-0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width,
			nearplane_distance, eye_position.z, render_vertices, image_w, image_h);
		for (std::map<my_3D_point_coord*, my_draw_color*>::iterator piter =
			render_vertices.begin(); piter != render_vertices.end(); piter++)
		{
			my_3Dvector raydir(eye_position, *(piter->first));
			raydir.normalized();
			my_draw_color newColor2 = one_ray_trace_my(eye_position, raydir, cur3DModel,
				0, eye_position, light_position, light_rgb_ambient, light_rgb_diffuse_specular);
			*(piter->second) = newColor2;
		}
		rendered = true;
		glutPostRedisplay();
		break;
	}
		

	case 27:
		exit(0);
		break;
	}
}

//ͶӰ��ʽ��modelview ��ʽ���á���ͶӰ�����
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	render_vertices.clear();
	if (w <= h)
	{
		//����͸���Ӿ���
		glFrustum(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, 0.5 * nearplane_height * (GLfloat)nearplane_height / (GLfloat)nearplane_width, nearplane_distance, farplane_distance); //������ӵ�
	}
	else
	{
		//����͸���Ӿ���
		glFrustum(-0.5 * nearplane_width, 0.5 * nearplane_width, -0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, 0.5 * nearplane_height * (GLfloat)nearplane_width / (GLfloat)nearplane_height, nearplane_distance, farplane_distance);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(eye_position.x, eye_position.y, eye_position.z, 0, 0, 0, 0, 1, 0);
}
//��꽻���¼�
//������ı��ӵ㷽��
void mouse(int button, int state, int x, int y) {
	switch (button) {
	case GLUT_LEFT_BUTTON:
		if (state == GLUT_DOWN) {
			eye_position.x = eye_position.x * cosf(-theta) + eye_position.z * sinf(-theta);
			eye_position.z = eye_position.z * cosf(-theta) - eye_position.x * sinf(-theta);
			eye_position.y = 1;
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
		break;
	case GLUT_RIGHT_BUTTON:
		if (state == GLUT_DOWN) {
			eye_position.x = eye_position.x * cosf(theta) + eye_position.z * sinf(theta);
			eye_position.z = eye_position.z * cosf(theta) - eye_position.x * sinf(theta);
			eye_position.y = 1;
			reshape(nearplane_width, nearplane_height);
			glutPostRedisplay();
		}
		break;
	default:
		break;
	}
 }
//��������
int main(int argc, char** argv)
{
	cout << "************************************************" << endl;
	cout << "*" << "W A S D F G�������ģ���ƶ�" << endl;
	cout << "*" << "H B N M������  �����ƶ�" << endl;
	cout << "*" << "P������������   ��λ" << endl;
	cout << "*" << "Q E����������   ���ģ����ת" << endl;
	cout << "*" << "1 2����������   ���������" << endl;
	cout << "*" << "3 4���������������Դλ��" << endl;
	cout << "*" << "0 9������������ѡ�����" << endl;
	cout << endl;
	cout << "*" << "Z C����������������Z��Buffer" << endl;
	cout << "*" << "L K���������������ص��Դ" << endl;
	cout << "*" << "T U��������������������" << endl;
	cout << "*" << "R����������������Ӱ" << endl;
	cout << "*" << "J������������������׷��" << endl;
	cout << "************************************************" << endl;
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(nearplane_width, nearplane_height);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("������ջ��");
	init();
	glutReshapeFunc(reshape);
	glutDisplayFunc(display);
	glutKeyboardFunc(keyboard);
	glutMouseFunc(mouse);
	glutMainLoop();

	return 0;
}