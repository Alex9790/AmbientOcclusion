#include "Globales.h"


void reshape(int width, int height)
{
	viewport_width = width;
    viewport_height = height;

	if(height == 0) //Prevenimos dividir entre 0
		height = 1;
		
	V = glm::lookAt(glm::vec3(0.0f,3.0f,3.0f),glm::vec3(0.0f,0.0f,0.0f),glm::vec3(0.0f,1.0f,0.0f));
	P = glm::perspective(60.0f, (GLfloat)(width/height), 0.01f, 1000.0f);

	// Cambiar tamaño del viewport
	glViewport(0, 0, width, height);
	TwWindowSize(width, height);
	WWidth = width;
	WHeight = height;
//	RTT_WIDTH = WWidth;
//	RTT_HEIGHT = WHeight;
}

void PosicionObjeto(Shaders * Shader, glm::vec3 Pos, bool M_Matrix, bool MV_Matrix, bool N_Matrix, bool P_Matrix, bool MVP_Matrix){

	//Limpiar Matrices
	transMatrix = glm::mat4(1.0f);
	RotMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	M = glm::mat4(1.0f);
	MV = glm::mat4(1.0f);
	MVP = glm::mat4(1.0f);

	//Trasladando Objeto
	transMatrix = glm::translate(Pos);
	// Multiply them together
	M =  transMatrix * RotMatrix * scaleMatrix;
	//ModelViewMatrix
	MV = V * M;
	//Model View Projection Matrix
	MVP = P * MV;
	if(M_Matrix)
		Shader->setUniform("ModelMatrix", M);
	if(MV_Matrix)
		Shader->setUniform("ModelViewMatrix", MV);
	if(N_Matrix)
		Shader->setUniform("NormalMatrix", glm::mat3( glm::vec3(MV[0]), glm::vec3(MV[1]), glm::vec3(MV[2]) ));
	if(P_Matrix)
		Shader->setUniform("ProjectionMatrix", P);
	if(MVP_Matrix)
		Shader->setUniform("MVP", MVP);
}

void setMaterial(Shaders * Shader, Objeto * Obj){
	Shader->setUniform("Material.Ka", Obj->Ka[0], Obj->Ka[1], Obj->Ka[2]);
	Shader->setUniform("Material.Kd", Obj->Kd[0], Obj->Kd[1], Obj->Kd[2]);
	Shader->setUniform("Material.Ks", Obj->Ks[0], Obj->Ks[1], Obj->Ks[2]);
	Shader->setUniform("Material.Shininess", 100.0f);
}

void setLuz(Shaders * Shader){
	//Directional Light
	if(DirLuz.Encendido){
		/*glBegin(GL_POINTS);
			glVertex3f(DirLuz.Pos[0], DirLuz.Pos[1], DirLuz.Pos[2]);				
		glEnd();*/
		PosicionObjeto(ShaderA, glm::vec3(DirLuz.Pos[0], DirLuz.Pos[1], DirLuz.Pos[2]), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &Luz);
		ShaderA->setUniform("TexBase", 0);
		Luz.render();

		Shader->setUniform("Light[0].Position", V * glm::vec4(DirLuz.Pos[0], DirLuz.Pos[1], DirLuz.Pos[2], 0.0f) );
		Shader->setUniform("Light[0].Intensity", glm::vec3(DirLuz.Intensidad));
	}else{
		Shader->setUniform("Light[0].Intensity", 0.0f, 0.0f, 0.0f);
	}

}

void MeasureTimeInit(){
	glGenQueries(1, &query);
	glBeginQuery(GL_TIME_ELAPSED,query);
}

void MeasureTimeEnd(){
	glEndQuery(GL_TIME_ELAPSED);

	// retrieving the recorded elapsed time
	// wait until the query result is available
	while (!done) {
		glGetQueryObjectiv(query, 
				GL_QUERY_RESULT_AVAILABLE, 
				&done);
	}
 
	// get the query result
	glGetQueryObjectui64v(query, GL_QUERY_RESULT, &elapsed_time);
	printf("Time Elapsed: %f ms\n", elapsed_time / 1000000.0);
}

void SSAOPrimerPaso(){
//bind the shader 
		ShaderPrimerPasoSSAO->Enable();
			//set the shader uniforms
			ShaderPrimerPasoSSAO->setUniform("MVP", MVP);

//////// INICIO OBJETOS A APLICAR AMBIENT OCCLUSION	///////////////			
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(0.0, 0.0, 0.0), 0, 0, 1, 0, 1);
/*			Grass.render();
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(0.0, 5.0, 0.0), 0, 0, 1, 0, 1);
			PortalGun.render();
*/
			Ground.render();
/*			Wolf.render();	
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(-20.0, 0.0, -10.0), 0, 0, 1, 0, 1);
			Blacksmith.render();

			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(20.0, 0.0, -15.0), 0, 0, 1, 0, 1);
			Alduin.render();
			AlduinEyes.render();	
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(10.0, 0.0, 0.0), 0, 0, 1, 0, 1);
			VanquishInnersuit1.render();
			VanquishInnersuit2.render();
			VanquishOutersuitlower.render();
			VanquishOutersuitupper.render();
			VanquishOutersuitupper2.render();
			VanquishVisor.render();
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(0.0, 0.0, -15.0), 0, 0, 1, 0, 1);
			Porsche1.render();
			Porsche2.render();
*/
/*
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(0.0, 0.0, -10.0), 0, 0, 1, 0, 1);
			Town1.render();
			Town2.render();
			Town3.render();
*/
			PosicionObjeto(ShaderPrimerPasoSSAO, glm::vec3(0.0, 0.0, -15.0), 0, 0, 1, 0, 1);
			if(Escena1)
			{
				Blacksmith.render();
			}

			if(Escena2)
			{
				Alduin.render();
				AlduinEyes.render();
			}

			if(Escena3)
			{
				Town1.render();
				Town2.render();
				Town3.render();
			}

//////// FIN OBJETOS A APLICAR AMBIENT OCCLUSION	///////////////
		//unbind the first step shader			
		ShaderPrimerPasoSSAO->Disable();
}

void AlchemyAO_Render(){
	//set the filtering FBO
	//glBindFramebuffer(GL_FRAMEBUFFER,filterFBOID);
	//set colour attachment 0 as the draw buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//bind the fullscreen quad vertex array object
	glBindVertexArray(quadVAOID);
	//bind the second step SSAO shader
	AlchemyAO->Enable();
		//set shader uniforms
 			
		AlchemyAO->setUniform("NUM_SPIRAL_TURNS", NUM_SPIRAL_TURNS);
		AlchemyAO->setUniform("NUM_SAMPLES", NUM_SAMPLES);
		AlchemyAO->setUniform("aoRad", aoRad);
		AlchemyAO->setUniform("aoBias", aoBias);
		AlchemyAO->setUniform("VerAO", ActivarVerAlchemyAO);

		//draw fullscreen quad
//			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_TRIANGLES,0,6);

	//capturar framebuffer de normales
	//frameGrab.CaptureFrame();

	//unbind the second step SSAO shader
	AlchemyAO->Disable();
}

void SAO_Render(){
	//set the filtering FBO
	//glBindFramebuffer(GL_FRAMEBUFFER,filterFBOID);
	//set colour attachment 0 as the draw buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//bind the fullscreen quad vertex array object
	glBindVertexArray(quadVAOID);
	//bind the second step SSAO shader
	SAO->Enable();
		//set shader uniforms	

		SAO->setUniform("NUM_SAMPLES", SAONUM_SAMPLES);
		SAO->setUniform("NUM_SPIRAL_TURNS", SAONUM_SPIRAL_TURNS);
		SAO->setUniform("VARIATION", SAOVARIATION);
		//SAO->setUniform("intensity", intensity);
		SAO->setUniform("sampleRadiusWS", SAOradius);
		SAO->setUniform("bias", SAObias);
		SAO->setUniform("zNear", SAOzNear);
		SAO->setUniform("zFar", SAOzFar);
		SAO->setUniform("projScale", SAOprojScale);
		SAO->setUniform("intensityDivR6", (SAOintensity / (pow(SAOradius, 6))));

		glDrawArrays(GL_TRIANGLES,0,6);

	//capturar framebuffer de normales
	//frameGrab.CaptureFrame();

	//unbind the second step SSAO shader
	SAO->Disable();
}

void UnrealEngineAO_Render(){
	//set colour attachment 0 as the draw buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	//bind the fullscreen quad vertex array object
	glBindVertexArray(quadVAOID);
	//bind the second step SSAO shader
	UnrealEngineAO->Enable();
		//set shader uniforms	

		UnrealEngineAO->setUniform("inverse_view_projection_matrix", glm::inverse(P * V));
		UnrealEngineAO->setUniform("wc_camera_eye_position", glm::vec3(Cam.m_x, Cam.m_y, Cam.m_z));
				
		UnrealEngineAO->setUniform("NUM_SAMPLE_DIRECTIONS", NUM_SAMPLE_DIRECTIONS);
		//UnrealEngineAO->setUniform("NUM_SAMPLE_STEPS", NUM_SAMPLE_STEPS);
		//UnrealEngineAO->setUniform("APPLY_ATTENUATION", APPLY_ATTENUATION);
		//UnrealEngineAO->setUniform("USE_ACTUAL_NORMALS", USE_ACTUAL_NORMALS);
		//UnrealEngineAO->setUniform("uFOV", uFOV);
		UnrealEngineAO->setUniform("uSampleRadius", uSampleRadius);
		UnrealEngineAO->setUniform("uAngleBias", uAngleBias);
		UnrealEngineAO->setUniform("uIntensity", uIntensity);

		//draw fullscreen quad
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_TRIANGLES,0,6);

	//capturar framebuffer de normales
	//frameGrab.CaptureFrame();

	//unbind the second step SSAO shader
	UnrealEngineAO->Disable();
}

void Blur(){
	//glBindFramebuffer(GL_FRAMEBUFFER,filterFBOID);
	//set colour attachment 1 as the draw buffer
	glDrawBuffer(GL_COLOR_ATTACHMENT1);

	//render full screen quad again with the vertical Gaussian smoothing shader
	glBindVertexArray(quadVAOID);
	ShaderGaussV->Enable();
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_TRIANGLES,0,6);		
	ShaderGaussV->Disable();

	//set colour attachment 2 as the draw buffer and render full screen quad 
	//again with the horizontal Gaussian smoothing shader
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	ShaderGaussH->Enable();
//		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
		glDrawArrays(GL_TRIANGLES,0,6);
	ShaderGaussH->Disable();
}

void SSAO_Render(){
	//seleccionar las texturas
		AlchemyAO->Enable();
			//AlchemyAO->setUniform("normalTex", 1);			//valor inicial = 1
			//AlchemyAO->setUniform("noiseTex", 2);
			AlchemyAO->setUniform("depthTex", 3);			//valor inicial = 3
		AlchemyAO->Disable();

		ShaderGaussV->Enable();
			ShaderGaussV->setUniform("textureMap", 4);		//valor inicial = 4
		ShaderGaussV->Disable();

		ShaderFinal->Enable();
			ShaderFinal->setUniform("textureMap", 4);		//valor inicial = 4
		ShaderFinal->Disable();

		ShaderGaussH->Enable();
			ShaderGaussH->setUniform("textureMap", 5);		//valor inicial = 5
		ShaderGaussH->Disable();

	//bind the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, fboID);
		//set the viewport to the size of the offscreen render target
		glViewport(0,0,RTT_WIDTH,RTT_HEIGHT);
		//set the colour attachment 0 ad the draw buffer
		//glDrawBuffer(GL_COLOR_ATTACHMENT0);
		//clear the colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Shader Primer Paso
		SSAOPrimerPaso();
	
		glBindFramebuffer(GL_FRAMEBUFFER,filterFBOID);

//		MeasureTimeInit();
//		cout<<"Ambient Occlusion"<<endl;

		if(ActivarAlchemyAO) {
//////// INICIO ALCHEMY AO	///////////////
//			cout<<"Alchemy AO"<<endl;
			AlchemyAO_Render();
//////// FIN ALCHEMY AO	///////////////
		}

		if(ActivarSAO) {
//////// INICIO SAO	///////////////
//			cout<<"Scalable AO"<<endl;
			SAO_Render();
//////// FIN SAO	///////////////
		}

		if(ActivarUnrealEngineAO) {
//////// INICIO UnrealEngineAO	///////////////
//			cout<<"Unreal Engine AO"<<endl;
			UnrealEngineAO_Render();
//////// FIN UnrealEngineAO	///////////////
		}

//		MeasureTimeEnd();

		if(ActivarBLUR) {
//			MeasureTimeInit();
//////// INICIO BLUR	///////////////
//			cout<<"Blur"<<endl;
			Blur();
//////// INICIO BLUR	///////////////
//			MeasureTimeEnd();
		}

//		cout<<"Final"<<endl; cout<<"Width: "<<RTT_WIDTH<<endl;
//		MeasureTimeInit();

		//unbind FBO, restore the defaul viewport and draw buffer
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glViewport(0,0,WWidth, WHeight);
		glDrawBuffer(GL_BACK_LEFT);

		//now draw the final filtered SSAO result with blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		ShaderFinal->Enable();
			ShaderFinal->setUniform("textureMap", 4);
			glDrawArrays(GL_TRIANGLES,0,6);
		ShaderFinal->Disable();
		
		//disable blending
		glDisable(GL_BLEND);

//		MeasureTimeEnd();

}

void MSSAO_Render() {
	int contador = 6;				//6 10 14 18 22
	for (int i = 0; i < LEVEL_COUNT; i++)		//LEVEL_COUNT																	
	{
	//seleccionar las texturas
		AlchemyAO->Enable();
			//AlchemyAO->setUniform("normalTex", contador);			//valor inicial = 6
			contador++;
			//AlchemyAO->setUniform("noiseTex", 2);
			AlchemyAO->setUniform("depthTex", contador);			//valor inicial = 7
			contador++;
		AlchemyAO->Disable();

		ShaderGaussV->Enable();
			ShaderGaussV->setUniform("textureMap", contador);		//valor inicial = 8
		ShaderGaussV->Disable();

		ShaderFinal->Enable();
			ShaderFinal->setUniform("textureMap", contador);		//valor inicial = 8
			contador++;
		ShaderFinal->Disable();

		ShaderGaussH->Enable();
			ShaderGaussH->setUniform("textureMap", contador);		//valor inicial = 9
			contador++;
		ShaderGaussH->Disable();

	//bind the FBO
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufs[i]);
		//set the viewport to the size of the offscreen render target
		glViewport(0,0,RTT_WIDTH/(pow(2,i)),RTT_HEIGHT/(pow(2,i)));
		//set the colour attachment 0 ad the draw buffer
		glDrawBuffer(GL_COLOR_ATTACHMENT0);
		//clear the colour and depth buffers
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

		//Shader Primer Paso
		SSAOPrimerPaso();		
	
		glBindFramebuffer(GL_FRAMEBUFFER,filterframeBufs[i]);

//		MeasureTimeInit();

		if(MSSAO_ActivarAlchemyAO) {
//			cout<<"MultiResolution AO"<<endl;
//////// INICIO ALCHEMY AO	///////////////
			//set the filtering FBO
			//set colour attachment 0 as the draw buffer
			glDrawBuffer(GL_COLOR_ATTACHMENT0);
			//bind the fullscreen quad vertex array object
			glBindVertexArray(quadVAOID);
			//bind the second step SSAO shader
			AlchemyAO->Enable();
				//set shader uniforms
  			
				AlchemyAO->setUniform("NUM_SPIRAL_TURNS", MSSAO_NUM_SPIRAL_TURNS);
				AlchemyAO->setUniform("NUM_SAMPLES", MSSAO_NUM_SAMPLES);
				AlchemyAO->setUniform("aoRad", MSSAO_aoRad);
				AlchemyAO->setUniform("aoBias", MSSAO_aoBias);

				//draw fullscreen quad
				glDrawArrays(GL_TRIANGLES,0,6);

			//unbind the second step SSAO shader
			AlchemyAO->Disable();
//////// FIN ALCHEMY AO	///////////////
		}

//		MeasureTimeEnd();
		SumMSSAOTime += elapsed_time;

		if(ActivarBLUR) {
//			MeasureTimeInit();
//////// INICIO BLUR	///////////////
//			cout<<"Blur"<<endl;
			Blur();
//////// INICIO BLUR	///////////////
//			MeasureTimeEnd();
			SumMSSAOBlurTime+= elapsed_time;
		}	
	}
		//cout<<"Time Elapsed MSSAO Total: "<< SumMSSAOTime / 1000000.0 <<endl;
//		printf("Time Elapsed MSSAO Total: %f ms\n", SumMSSAOTime / 1000000.0);
		SumMSSAOTime = 0.0;
//		printf("Time Elapsed MSSAO Blur Total: %f ms\n", SumMSSAOBlurTime / 1000000.0);
		SumMSSAOBlurTime = 0.0;

//		cout<<"Final"<<endl;
//		MeasureTimeInit();

		//unbind FBO, restore the defaul viewport and draw buffer
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glViewport(0,0,WWidth, WHeight);
		glDrawBuffer(GL_BACK_LEFT);

		//now draw the final filtered SSAO result with blending
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);
		ShaderFinalMSSAO->Enable();
			ShaderFinalMSSAO->setUniform("esMSSAO", true);
			ShaderFinalMSSAO->setUniform("textureMap", 8);		//8 12 16 20 24
			ShaderFinalMSSAO->setUniform("textureMap2", 12);
			ShaderFinalMSSAO->setUniform("textureMap3", 16);
			ShaderFinalMSSAO->setUniform("textureMap4", 20);
			ShaderFinalMSSAO->setUniform("textureMap5", 24);
			
			ShaderFinalMSSAO->setUniform("intensidadTex1", intensidadTex1);
			ShaderFinalMSSAO->setUniform("intensidadTex2", intensidadTex2);
			ShaderFinalMSSAO->setUniform("intensidadTex3", intensidadTex3);
			ShaderFinalMSSAO->setUniform("intensidadTex4", intensidadTex4);
			ShaderFinalMSSAO->setUniform("intensidadTex5", intensidadTex5);

			glDrawArrays(GL_TRIANGLES,0,6);
		ShaderFinalMSSAO->Disable();
		
		//disable blending
		glDisable(GL_BLEND);

//		MeasureTimeEnd();
}

void Dibujar(){

	Cam.Refresh();
	V = glm::lookAt(glm::vec3(Cam.m_x, Cam.m_y, Cam.m_z),
					glm::vec3(Cam.m_x + Cam.m_lx, Cam.m_y + Cam.m_ly, Cam.m_z + Cam.m_lz),
					glm::vec3(0.0, 1.0, 0.0));

	//Inicio medicion tiempo
//	MeasureTimeInit();

	ShaderA->Enable();
	
	////Dibujando//////	
		setLuz(ShaderA);
//		cout<<"Render Objects:"<<endl;

		//Objetos
		//Skybox
		PosicionObjeto(ShaderA, glm::vec3(Cam.m_x, Cam.m_y, Cam.m_z), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &Cube);
		ShaderA->setUniform("CubeMapTex", 3);
		ShaderA->setUniform("DrawSkyBox", true);
		Cube.render();
		ShaderA->setUniform("DrawSkyBox", false);

		//Multi-Texture
		PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, 0.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &Ground);
		ShaderA->setUniform("TexBase", 0);
		ShaderA->setUniform("TexMulti", 1);
		Ground.render();

		/*
		PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, 0.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &Grass);
		ShaderA->setUniform("TexBase", 0);
		Grass.render();

		PosicionObjeto(ShaderA, glm::vec3(0.0, 5.0, 0.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &PortalGun);
		ShaderA->setUniform("TexBase", 0);
		PortalGun.render();
*/
/*
		PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, 0.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &Wolf);
		ShaderA->setUniform("TexBase", 0);
		Wolf.render();
*/
		if(Escena1)
		{
			//PosicionObjeto(ShaderA, glm::vec3(20.0, 0.0, -15.0), 0, 1, 1, 0, 1);
			PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, -15.0), 0, 1, 1, 0, 1);
			setMaterial(ShaderA, &Blacksmith);
			ShaderA->setUniform("TexBase", 0);
			Blacksmith.render();
		}

		if(Escena2)
		{
			//PosicionObjeto(ShaderA, glm::vec3(20.0, 0.0, -15.0), 0, 1, 1, 0, 1);
			PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, -15.0), 0, 1, 1, 0, 1);
			setMaterial(ShaderA, &Alduin);
			ShaderA->setUniform("TexBase", 0);
			Alduin.render();
			setMaterial(ShaderA, &AlduinEyes);
			ShaderA->setUniform("TexBase", 0);
			AlduinEyes.render();
		}
/*
		PosicionObjeto(ShaderA, glm::vec3(10.0, 0.0, 0.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &VanquishInnersuit1);
		VanquishInnersuit1.render();
		setMaterial(ShaderA, &VanquishInnersuit2);
		VanquishInnersuit2.render();
		setMaterial(ShaderA, &VanquishOutersuitlower);
		VanquishOutersuitlower.render();
		setMaterial(ShaderA, &VanquishOutersuitupper);
		VanquishOutersuitupper.render();
		setMaterial(ShaderA, &VanquishOutersuitupper2);
		VanquishOutersuitupper2.render();
		setMaterial(ShaderA, &VanquishVisor);
		VanquishVisor.render();

		PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, -15.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &Porsche1);
		ShaderA->setUniform("TexBase", 0);
		Porsche1.render();
		setMaterial(ShaderA, &Porsche2);
		ShaderA->setUniform("TexBase", 0);
		Porsche2.render();
*/
		if(Escena3)
		{
			PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, -15.0), 0, 1, 1, 0, 1);
			setMaterial(ShaderA, &Town1);
			ShaderA->setUniform("TexBase", 0);
			Town1.render();
			setMaterial(ShaderA, &Town2);
			ShaderA->setUniform("TexBase", 0);
			Town2.render();
			setMaterial(ShaderA, &Town3);
			ShaderA->setUniform("TexBase", 0);
			Town3.render();
		}
/*		PosicionObjeto(ShaderA, glm::vec3(0.0, 0.0, 0.0), 0, 1, 1, 0, 1);
		setMaterial(ShaderA, &CrytekSponza);
		CrytekSponza.render();
*/
			
	ShaderA->Disable();

	//Fin medicion tiempo
//	MeasureTimeEnd();

	//if SSAO is enabled
	if(ActivarSSAO) {
		ShaderA->Enable();
			ShaderA->setUniform("VerSoloAO", VerSoloAO);
		ShaderA->Disable();
		//Render del AO, recordar colocar los objetos a occluir dentro del render, en la primera pasada de SSAO
		//if MSSAO is enabled
		if(ActivarMSSAO) {
			//Render del AO, recordar colocar los objetos a occluir dentro del render, en la primera pasada de SSAO
			MSSAO_Render();
		}else{
			SSAO_Render();
		}
	}

}

void renderScene(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	Dibujar();

	if(ScreenShot){
		//capturar framebuffer de normales
		frameGrab.CaptureFrame();
		ScreenShot = false;
	}
	TwDraw();

	glutSwapBuffers();
	glutPostRedisplay();

/*	GLenum error = glGetError(); 

	if (error != GL_NO_ERROR){
		std::cout << "[OpenGL ERROR]: " << error << " " << gluErrorString(error) << std::endl;
		assert(error != GL_NO_ERROR);
		system("pause");
		exit(0);
	}
*/
}

/* ====================================////////////////////////////////================
											INTERACCIONES
=======================================////////////////////////////////==============*/
void keyboard(unsigned char key,int x, int y)
{
	if(	!TwEventKeyboardGLUT(key,x,y)){
		if(key==27)	{
			TwTerminate();
			exit(0);
		}
	}
//	glutPostRedisplay();

	if(key == ' ') {
        fps_mode = !fps_mode;

        if(fps_mode) {
            glutSetCursor(GLUT_CURSOR_NONE);
            glutWarpPointer(viewport_width/2, viewport_height/2);
        }
        else {
            glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
        }
    }

    g_key[key] = true;

}

void KeyboardUp(unsigned char key, int x, int y)
{
    g_key[key] = false;
}

void left_click(int boton, int estado, int x, int y) 
{
	if( !TwEventMouseButtonGLUT(boton,estado,x,y) )  // send event to AntTweakBar
	{
	}

	if(estado == GLUT_DOWN) {
        if(boton == GLUT_LEFT_BUTTON) {
            mouse_left_down = true;
        }
        else if(boton == GLUT_RIGHT_BUTTON) {
            mouse_right_down = true;
        }
    }
    else if(estado == GLUT_UP) {
        if(boton == GLUT_LEFT_BUTTON) {
            mouse_left_down = false;
        }
        else if(boton == GLUT_RIGHT_BUTTON) {
            mouse_right_down = false;
        }
    }

	//glutPostRedisplay();
}

void movimiento(int x,int y){
	if(!TwEventMouseMotionGLUT(x,y) ){
	}

	// This variable is hack to stop glutWarpPointer from triggering an event callback to Mouse(...)
    // This avoids it being called recursively and hanging up the event loop
    static bool just_warped = false;

    if(just_warped) {
        just_warped = false;
        return;
    }

    if(fps_mode) {
        int dx = x - viewport_width/2;
        int dy = y - viewport_height/2;

        if(dx) {
            Cam.RotateYaw(rotation_speed*dx);
        }

        if(dy) {
            Cam.RotatePitch(rotation_speed*(-dy));
        }

        glutWarpPointer(viewport_width/2, viewport_height/2);

        just_warped = true;
    }

	//glutPostRedisplay();
}

void Timer(int value)
{
    if(fps_mode) {
        if(g_key['w'] || g_key['W']) {
            Cam.Move(translation_speed);
        }
        else if(g_key['s'] || g_key['S']) {
            Cam.Move(-translation_speed);
        }
        else if(g_key['a'] || g_key['A']) {
            Cam.Strafe(translation_speed);
        }
        else if(g_key['d'] || g_key['D']) {
            Cam.Strafe(-translation_speed);
        }
        else if(mouse_left_down) {
            Cam.Fly(-translation_speed);
        }
        else if(mouse_right_down) {
            Cam.Fly(translation_speed);
        }
    }

    glutTimerFunc(1, Timer, 0);
}

/* ====================================////////////////////////////////================
											INICIALIZADORES
=======================================////////////////////////////////==============*/

void initVar(){
	traslacion.x = 0.0f;
	traslacion.y = 0.0f;
	traslacion.z = 0.0f;

	escalamiento.x = 1.0f;
	escalamiento.y = 1.0f;
	escalamiento.z = 1.0f;

	//inicializar bool de luces
	//luz Direccional
	DirLuz.Encendido = true;
	DirLuz.Pos[0] = 0.0f; DirLuz.Pos[1] = 30.0f; DirLuz.Pos[2] = 0.0f;
	DirLuz.Intensidad = 2.0f;
} 

void TW_CALL LuzDir(void *clientData) 
{ 
	if(DirLuz.Encendido){
		DirLuz.Encendido = false;
	}else{ 
		DirLuz.Encendido = true;
	} 
} 
void TW_CALL SSAOFunc(void *clientData) 
{ 
	if(ActivarSSAO){
		ActivarSSAO = false;
	}else{ 
		ActivarSSAO = true;
	} 
}
void TW_CALL BLURFunc(void *clientData) 
{ 
	if(ActivarBLUR){
		ActivarBLUR = false;
	}else{ 
		ActivarBLUR = true;
	} 
}
void TW_CALL VerAOFunc(void *clientData) 
{ 
	if(VerSoloAO){
		VerSoloAO = false;
	}else{ 
		VerSoloAO = true;
	} 
}
void TW_CALL AlchemyAOFunc(void *clientData) 
{ 
	if(ActivarAlchemyAO){
		ActivarAlchemyAO = false;
	}else{ 
		ActivarAlchemyAO = true;
		ActivarSAO = false;
		ActivarUnrealEngineAO = false;
		ActivarMSSAO = false;
	} 
}
void TW_CALL VerAlchemyAOFunc(void *clientData) 
{ 
	if(ActivarVerAlchemyAO){
		ActivarVerAlchemyAO = false;
	}else{ 
		ActivarVerAlchemyAO = true;
	}
}
void TW_CALL SAOFunc(void *clientData) 
{ 
	if(ActivarSAO){
		ActivarSAO = false;
	}else{ 
		ActivarSAO = true;
		ActivarAlchemyAO = false;
		ActivarUnrealEngineAO = false;
		ActivarMSSAO = false;
	} 
}
void TW_CALL UnrealEngineAOFunc(void *clientData) 
{ 
	if(ActivarUnrealEngineAO){
		ActivarUnrealEngineAO = false;
	}else{ 
		ActivarUnrealEngineAO = true;
		ActivarAlchemyAO = false;
		ActivarSAO = false;
		ActivarMSSAO = false;
	} 
}
void TW_CALL MSSAOFunc(void *clientData) 
{ 
	if(ActivarMSSAO){
		ActivarMSSAO = false;
	}else{ 
		ActivarMSSAO = true;
		ActivarAlchemyAO = false;
		ActivarSAO = false;
		ActivarUnrealEngineAO = false;
	} 
}
void TW_CALL Escena1Func(void *clientData) 
{ 
	Escena1 = true;
	Escena2 = false;
	Escena3 = false;
}
void TW_CALL Escena2Func(void *clientData) 
{ 
	Escena1 = false;
	Escena2 = true;
	Escena3 = false;
}
void TW_CALL Escena3Func(void *clientData) 
{ 
	Escena1 = false;
	Escena2 = false;
	Escena3 = true;
}
void TW_CALL ScreenShotFunc(void *clientData) 
{ 
	//capturar framebuffer de normales
	//frameGrab.CaptureFrame();
	ScreenShot = true;
}
void TW_CALL GetCamFunc(void *clientData) 
{ 
	CamPosX = Cam.m_x;
	CamPosY = Cam.m_y; 
	CamPosZ = Cam.m_z;
	CamDirX = Cam.m_lx;
	CamDirY = Cam.m_ly;
	CamDirZ = Cam.m_lz;
	Cam.GetYaw(CamYaw);
	Cam.GetPitch(CamPitch);
}
void TW_CALL SetCamFunc(void *clientData) 
{ 
	Cam.SetCamera(CamPosX, CamPosY, CamPosZ, CamDirX, CamDirY, CamDirZ, CamYaw, CamPitch);
}

void initAntTweak(){
	TwInit(TW_OPENGL, NULL);
	TwWindowSize(800,600);
	glutPassiveMotionFunc((GLUTmousemotionfun)TwEventMouseMotionGLUT);
	TwGLUTModifiersFunc(glutGetModifiers);
	TwBar *myBar;
	myBar = TwNewBar("Opciones");
	TwAddSeparator(myBar,"Transformacion",NULL);

	TwAddButton(myBar,"Luz Direccional", LuzDir, NULL,"label='Encender' group='Luz Dir'");
	TwAddVarRW(myBar, "PosX", TW_TYPE_FLOAT, &DirLuz.Pos[0], "step=0.01 label='PosX' group='Luz Dir'");
	TwAddVarRW(myBar, "PosY", TW_TYPE_FLOAT, &DirLuz.Pos[1], "step=0.01 label='PosY' group='Luz Dir'");
	TwAddVarRW(myBar, "PosZ", TW_TYPE_FLOAT, &DirLuz.Pos[2], "step=0.01 label='PosZ' group='Luz Dir'");
	TwAddVarRW(myBar, "Intensidad", TW_TYPE_FLOAT, &DirLuz.Intensidad, "min=0.0 step=0.01 label='Intensidad' group='Luz Dir'");

//////////AMBIENT OCCLUSION
	TwAddButton(myBar,"Escena_1", Escena1Func, NULL,"label='Escena 1' group='Escenas'");
	TwAddButton(myBar,"Escena_2", Escena2Func, NULL,"label='Escena 2' group='Escenas'");
	TwAddButton(myBar,"Escena_3", Escena3Func, NULL,"label='Escena 3' group='Escenas'");
	TwAddButton(myBar,"ScreenShot", ScreenShotFunc, NULL,"label='ScreenCapture' group='Escenas'");

	TwAddButton(myBar,"Cam_1", GetCamFunc, NULL,"label='GetDataCamara' group='Camara'");
	TwAddButton(myBar,"Cam_2", SetCamFunc, NULL,"label='SetDataCamara' group='Camara'");
	TwAddVarRW(myBar, "Cam_3", TW_TYPE_FLOAT, &CamPosX, "step=0.001 label='CamPosX' group='Camara'");
	TwAddVarRW(myBar, "Cam_4", TW_TYPE_FLOAT, &CamPosY, "step=0.001 label='CamPosY' group='Camara'");
	TwAddVarRW(myBar, "Cam_5", TW_TYPE_FLOAT, &CamPosZ, "step=0.001 label='CamPosZ' group='Camara'");
	TwAddVarRW(myBar, "Cam_6", TW_TYPE_FLOAT, &CamDirX, "step=0.001 label='CamDirX' group='Camara'");
	TwAddVarRW(myBar, "Cam_7", TW_TYPE_FLOAT, &CamDirY, "step=0.001 label='CamDiry' group='Camara'");
	TwAddVarRW(myBar, "Cam_8", TW_TYPE_FLOAT, &CamDirZ, "step=0.001 label='CamDirZ' group='Camara'");
	TwAddVarRW(myBar, "Cam_9", TW_TYPE_FLOAT, &CamYaw, "step=0.001 label='CamYaw' group='Camara'");
	TwAddVarRW(myBar, "Cam_10", TW_TYPE_FLOAT, &CamPitch, "step=0.001 label='CamPitch' group='Camara'");

	TwAddButton(myBar,"Screen Space AO", SSAOFunc, NULL,"label='Activar SSAO' group='SSAO'");
	TwAddButton(myBar,"BLUR", BLURFunc, NULL,"label='Activar BLUR' group='SSAO'");
	TwAddButton(myBar,"VerSoloAO", VerAOFunc, NULL,"label='Ver Solo AO' group='SSAO'");
/*	TwAddVarRW(myBar, "RadioMuestreo1", TW_TYPE_FLOAT, &RadioMuestreo, "min=0.0 step=0.01 label='Radio Muestreo' group='SSAO'");
	TwAddVarRW(myBar, "Scale1", TW_TYPE_FLOAT, &Scale, "min=0.0 step=0.01 label='Radio Oclusion' group='SSAO'");
	TwAddVarRW(myBar, "Bias1", TW_TYPE_FLOAT, &Bias, "step=0.01 label='Iluminacion' group='SSAO'");
	TwAddVarRW(myBar, "Intensity1", TW_TYPE_FLOAT, &Intensity, "step=0.01 label='Oscuridad Sombra' group='SSAO'");
*/

	////ALCHEMY AO
	TwAddButton(myBar,"Alchemy AO", AlchemyAOFunc, NULL,"label='Activar Alchemy AO' group='AlchemyAO'");
//	TwAddButton(myBar,"Ver Alchemy AO", VerAlchemyAOFunc, NULL,"label='Ver Solo Alchemy AO' group='AlchemyAO'");
/*	TwAddVarRW(myBar, "base_samples1", TW_TYPE_INT32, &base_samples, "label='base samples' group='AlchemyAO'");
	TwAddVarRW(myBar, "min_samples1", TW_TYPE_INT32, &min_samples, "label='min samples' group='AlchemyAO'");
	TwAddVarRW(myBar, "radius1", TW_TYPE_FLOAT, &radius, "step=0.01 label='radius' group='AlchemyAO'");
	TwAddVarRW(myBar, "projection_factor1", TW_TYPE_FLOAT, &projection_factor, "step=0.001 label='projection_factor' group='AlchemyAO'");
	TwAddVarRW(myBar, "bias1", TW_TYPE_FLOAT, &bias, "step=0.01 label='bias' group='AlchemyAO'");
	TwAddVarRW(myBar, "sigma1", TW_TYPE_FLOAT, &sigma, "step=0.01 label='sigma' group='AlchemyAO'");*/
	TwAddVarRW(myBar, "NUM_SPIRAL_TURNS1", TW_TYPE_FLOAT, &NUM_SPIRAL_TURNS, "min=0.01 step=0.01 label='Spiral Turns' group='AlchemyAO'");
	TwAddVarRW(myBar, "NUM_SAMPLES1", TW_TYPE_FLOAT, &NUM_SAMPLES, "min=1.0 step=1.0 label='Samples' group='AlchemyAO'");
	TwAddVarRW(myBar, "aoRad1", TW_TYPE_FLOAT, &aoRad, "min=0.01 step=0.01 label='Radius' group='AlchemyAO'");
	TwAddVarRW(myBar, "aoBias1", TW_TYPE_FLOAT, &aoBias, "step=0.001 label='Bias' group='AlchemyAO'");

	////SAO
	TwAddButton(myBar,"Scalable AO", SAOFunc, NULL,"label='Activar SAO' group='SAO'");
	TwAddVarRW(myBar, "SAONUM_SAMPLES1", TW_TYPE_INT32, &SAONUM_SAMPLES, "step=1 min=0 label='samples' group='SAO'");
	TwAddVarRW(myBar, "SAONUM_SPIRAL_TURNS1", TW_TYPE_INT32, &SAONUM_SPIRAL_TURNS, "step=1 min=0 label='Spiral Turns' group='SAO'");
//	TwAddVarRW(myBar, "SAOVARIATION1", TW_TYPE_INT32, &SAOVARIATION, "step=1 min=0 max=3 label='Variation' group='SAO'");
	TwAddVarRW(myBar, "intensity1", TW_TYPE_FLOAT, &SAOintensity, "step=0.01 label='Intensity' group='SAO'");
	TwAddVarRW(myBar, "sampleRadiusWS1", TW_TYPE_FLOAT, &SAOradius, "step=0.01 label='Radius' group='SAO'");
	TwAddVarRW(myBar, "bias1", TW_TYPE_FLOAT, &SAObias, "step=0.001 label='Bias' group='SAO'");
	TwAddVarRW(myBar, "zNear1", TW_TYPE_FLOAT, &SAOzNear, "step=0.01 label='zNear' group='SAO'");
	TwAddVarRW(myBar, "zFar1", TW_TYPE_FLOAT, &SAOzFar, "step=0.01 label='zFar' group='SAO'");
	TwAddVarRW(myBar, "projScale1", TW_TYPE_FLOAT, &SAOprojScale, "step=0.01 label='ProjScale' group='SAO'");

	////UnrealEngineAO
	TwAddButton(myBar,"Unreal Engine AO", UnrealEngineAOFunc, NULL,"label='Activar Unreal Engine AO' group='UnrealEngineAO'");
	TwAddVarRW(myBar, "UNREALNUM_SAMPLES", TW_TYPE_INT32, &NUM_SAMPLE_DIRECTIONS, "step=1 min=1 label='Samples' group='UnrealEngineAO'");
//	TwAddVarRW(myBar, "UNREALNUM_STEPS", TW_TYPE_INT32, &NUM_SAMPLE_STEPS, "step=1 min=1 label='NUM_STEPS' group='UnrealEngineAO'");
//	TwAddVarRW(myBar, "UNREALAPPLY_ATTENUATION", TW_TYPE_INT32, &APPLY_ATTENUATION, "step=1 min=0 max=1 label='APPLY_ATTENUATION' group='UnrealEngineAO'");
//	TwAddVarRW(myBar, "UNREALuFOV", TW_TYPE_FLOAT, &uFOV, "step=0.01 min=0 label='uFOV' group='UnrealEngineAO'");
//	TwAddVarRW(myBar, "UNREALuSampleRadius", TW_TYPE_FLOAT, &uSampleRadius, "step=0.01 label='RadioMuestra' group='UnrealEngineAO'");
	TwAddVarRW(myBar, "UNREALuAngleBias", TW_TYPE_FLOAT, &uAngleBias, "step=0.01 min=0 label='Angle' group='UnrealEngineAO'");
	TwAddVarRW(myBar, "UNREALuIntensity", TW_TYPE_FLOAT, &uIntensity, "step=0.01 label='Intensity' group='UnrealEngineAO'");

	////MSSAO
	TwAddButton(myBar,"Multireslution AO", MSSAOFunc, NULL,"label='Activar MSSAO' group='MSSAO'");
	TwAddVarRW(myBar, "MSSAO NUM_SPIRAL_TURNS2", TW_TYPE_FLOAT, &MSSAO_NUM_SPIRAL_TURNS, "min=0.01 step=0.01 label='Spiral Turns' group='MSSAO'");
	TwAddVarRW(myBar, "MSSAO NUM_SAMPLES2", TW_TYPE_FLOAT, &MSSAO_NUM_SAMPLES, "min=1.0 step=1.0 label='Samples' group='MSSAO'");
	TwAddVarRW(myBar, "MSSAO aoRad2", TW_TYPE_FLOAT, &MSSAO_aoRad, "min=0.01 step=0.01 label='Radius' group='MSSAO'");
	TwAddVarRW(myBar, "MSSAO aoBias2", TW_TYPE_FLOAT, &MSSAO_aoBias, "step=0.001 label='Bias' group='MSSAO'");
	TwAddVarRW(myBar, "Intensidad Tex1", TW_TYPE_FLOAT, &intensidadTex1, "step=0.01 label='Intensity Text 1' group='MSSAO'");
	TwAddVarRW(myBar, "Intensidad Tex2", TW_TYPE_FLOAT, &intensidadTex2, "step=0.01 label='Intensity Text 2' group='MSSAO'");
	TwAddVarRW(myBar, "Intensidad Tex3", TW_TYPE_FLOAT, &intensidadTex3, "step=0.01 label='Intensity Text 3' group='MSSAO'");
	TwAddVarRW(myBar, "Intensidad Tex4", TW_TYPE_FLOAT, &intensidadTex4, "step=0.01 label='Intensity Text 4' group='MSSAO'");
	TwAddVarRW(myBar, "Intensidad Tex5", TW_TYPE_FLOAT, &intensidadTex5, "step=0.01 label='Intensity Text 5' group='MSSAO'");

}

void initObjetos(){

	Cube.LeerOBJ("files/Cube/", "Cube.obj");
	Cube.CrearTexturas();
	Cube.CrearCubeMap("files/Cube/Cube");

	Ground.LeerOBJ("files/Ground/", "Ground2.obj");
	Ground.CrearTexturas();
	Ground.CrearOtraTextura("files/Ground/moss.jpg");

	Wolf.LeerOBJ("files/Wolf/", "Wolf.obj");
	Wolf.CrearTexturas();

	Luz.LeerOBJ("files/Luz/", "Luz.obj");
	Luz.CrearTexturas();

	Grass.LeerOBJ("files/Grass/", "Grass_02.obj");
	Grass.CrearTexturas();

	PortalGun.LeerOBJ("files/Portal_Gun/", "Portal_Gun.obj");
	PortalGun.CrearTexturas();

	Blacksmith.LeerOBJ("files/Blacksmith/", "Blacksmith.obj");
	Blacksmith.CrearTexturas();

	Alduin.LeerOBJ("files/Alduin/", "Alduin.obj");
	Alduin.CrearTexturas();

	AlduinEyes.LeerOBJ("files/Alduin/", "AlduinEyes.obj");
	AlduinEyes.CrearTexturas();
/*
	CrytekSponza.LeerOBJ("files/CrytekSponza/", "CrytekSponza.obj");
	CrytekSponza.CrearTexturas();

	CrytekSponza.LeerOBJ("files/CrytekSponza/", "CrytekSponza.obj");
	CrytekSponza.CrearTexturas();
*/
	VanquishInnersuit1.LeerOBJ("files/Vanquish/Innersuit1/", "Innersuit1.obj");
	VanquishInnersuit1.CrearTexturas();

	VanquishInnersuit2.LeerOBJ("files/Vanquish/Innersuit2/", "Innersuit2.obj");
	VanquishInnersuit2.CrearTexturas();

	VanquishOutersuitlower.LeerOBJ("files/Vanquish/Outersuitlower/", "Outersuitlower.obj");
	VanquishOutersuitlower.CrearTexturas();

	VanquishOutersuitupper.LeerOBJ("files/Vanquish/Outersuitupper/", "Outersuitupper.obj");
	VanquishOutersuitupper.CrearTexturas();

	VanquishOutersuitupper2.LeerOBJ("files/Vanquish/Outersuitupper2/", "Outersuitupper.obj");
	VanquishOutersuitupper2.CrearTexturas();

	VanquishVisor.LeerOBJ("files/Vanquish/Visor/", "Visor.obj");
	VanquishVisor.CrearTexturas();

	Porsche1.LeerOBJ("files/Porsche/Porsche1/", "Porsche1.obj");
	Porsche1.CrearTexturas();

	Porsche2.LeerOBJ("files/Porsche/Porsche2/", "Porsche2.obj");
	Porsche2.CrearTexturas();

	Town1.LeerOBJ("files/Town/Town1/", "Town1.obj");
	Town1.CrearTexturas();

	Town2.LeerOBJ("files/Town/Town2/", "Town2.obj");
	Town2.CrearTexturas();

	Town3.LeerOBJ("files/Town/Town3/", "Town3.obj");
	Town3.CrearTexturas();

}

void initShaders(){
	//base
	ShaderA = new Shaders();
	if(ShaderA->isSuported()){
		ShaderA->Init("src/Shader/vertexShader.vsh","src/Shader/fragmentShader.fsh");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
}

void SSAO_Init(){
///////////// INICIO Textura Ruido	//////////////////
	//generate the pseudorandom noise data
	glm::vec4 pData[64][64];
	for(int j=0;j<64;j++) {
		for(int i=0;i<64;i++) {
			pData[i][j].x = (float)rand() / RAND_MAX;
			pData[i][j].y = (float)rand() / RAND_MAX;
			pData[i][j].z = (float)rand() / RAND_MAX;
			pData[i][j].w = (float)rand() / RAND_MAX;
		}
	}

	//use the pseudorandom noise data to generate a 64x64 nosie texture
	glGenTextures(1, &noiseTexID);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 64, 64, 0, GL_BGRA, GL_FLOAT, pData);

///////////// FIN Textura Ruido	//////////////////////	

///////////// INICIO SHADERS	//////////////////

	//final shader
	ShaderFinal = new Shaders();
	if(ShaderFinal->isSuported()){
		ShaderFinal->Init("src/Shader/SSAO/Passthrough.vert","src/Shader/SSAO/final.frag");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	ShaderFinal->Enable();
		ShaderFinal->setUniform("textureMap", 4);
	ShaderFinal->Disable();

	//horizontal Gaussian blurring shader
	ShaderGaussH = new Shaders();
	if(ShaderGaussH->isSuported()){
		ShaderGaussH->Init("src/Shader/SSAO/Passthrough.vert","src/Shader/SSAO/GaussH.frag");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	ShaderGaussH->Enable();
		ShaderGaussH->setUniform("textureMap", 5);
	ShaderGaussH->Disable();

	//vertical Gaussian blurring shader
	ShaderGaussV = new Shaders();
	if(ShaderGaussV->isSuported()){
		ShaderGaussV->Init("src/Shader/SSAO/Passthrough.vert","src/Shader/SSAO/GaussV.frag");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	ShaderGaussV->Enable();
		ShaderGaussV->setUniform("textureMap", 4);
	ShaderGaussV->Disable();

	//primer paso SSAO shader
	ShaderPrimerPasoSSAO = new Shaders();
	if(ShaderPrimerPasoSSAO->isSuported()){
		ShaderPrimerPasoSSAO->Init("src/Shader/SSAO/SSAO_FirstStep.vert","src/Shader/SSAO/SSAO_FirstStep.frag");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}

	//segundo paso SSAO shader
	ShaderSegundoPasoSSAO = new Shaders();
	if(ShaderSegundoPasoSSAO->isSuported()){
		ShaderSegundoPasoSSAO->Init("src/Shader/SSAO/Passthrough.vert","src/Shader/SSAO/SSAO_SecondStep.frag");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	ShaderSegundoPasoSSAO->Enable();
		ShaderSegundoPasoSSAO->setUniform("viewportSize", glm::vec2(float(RTT_WIDTH), float(RTT_HEIGHT) ));
		ShaderSegundoPasoSSAO->setUniform("invViewportSize", glm::vec2( 1.0f/float(RTT_WIDTH), 1.0f/float(RTT_HEIGHT) ));
		ShaderSegundoPasoSSAO->setUniform("normalTex", 1);
		ShaderSegundoPasoSSAO->setUniform("noiseTex", 2);
		ShaderSegundoPasoSSAO->setUniform("depthTex", 3);

		//matriz inversa de P
		glm::mat4 biasMat;
		biasMat = glm::translate(glm::mat4(1),glm::vec3(0.5,0.5,0.5));
		biasMat = glm::scale(biasMat, glm::vec3(0.5,0.5,0.5));
		glm::mat4 invP = biasMat*glm::inverse(P);
		ShaderSegundoPasoSSAO->setUniform("invP", invP);
		
		//calculando posicion de las muestras a tomar de cada pixel
		glm::vec2 samples[16];
		float angle = (float)M_PI_4;
		for(int i=0;i<16;i++) {
			samples[i].x = cos(angle) * (float)(i+1)/16.0f;
			samples[i].y = sin(angle) * (float)(i+1)/16.0f;
			angle += (float)M_PI_2;
			if(((i + 1) % 4) == 0)
				angle += (float)M_PI_4;
		}
		ShaderSegundoPasoSSAO->setUniform("samples", 16, samples);		
	ShaderSegundoPasoSSAO->Disable();

///////////// FIN SHADERS	//////////////////////

///////////// INICIO QUAD RENDER AO	//////////////////
	//fullscreen quad vertices donde rederizar el ambient occlusion para luego hacer blending con la escena
/*	glm::vec2 quadVerts[4];
	quadVerts[0] = glm::vec2(0,0);
	quadVerts[1] = glm::vec2(1,0);
	quadVerts[2] = glm::vec2(1,1);
	quadVerts[3] = glm::vec2(0,1);

	//quad indices
	GLushort quadIndices[]={ 0,1,2,0,2,3};
*/
	glm::vec2 quadVerts[6];
	quadVerts[0] = glm::vec2(0,0);
	quadVerts[1] = glm::vec2(1,0);
	quadVerts[2] = glm::vec2(1,1);
	quadVerts[3] = glm::vec2(0,0);
	quadVerts[4] = glm::vec2(1,1);
	quadVerts[5] = glm::vec2(0,1);

	//quad vertex array and vertex buffer objects
	glGenVertexArrays(1, &quadVAOID);
	glGenBuffers(1, &quadVBOID);
	glGenBuffers(1, &quadIndicesID);

	glBindVertexArray(quadVAOID);
	glBindBuffer (GL_ARRAY_BUFFER, quadVBOID);
	//pass quad vertices to vertex buffer object
	glBufferData (GL_ARRAY_BUFFER, sizeof(quadVerts), &quadVerts[0], GL_STATIC_DRAW);
	//enable vertex attribute array for vertex position	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE,0,0);
	glEnableVertexAttribArray(0);
	//pass quad indices to element array buffer
//	glBindBuffer (GL_ELEMENT_ARRAY_BUFFER, quadIndicesID);
//	glBufferData (GL_ELEMENT_ARRAY_BUFFER, sizeof(quadIndices), &quadIndices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);

///////////// FIN QUAD RENDER AO	//////////////////////

///////////// INICIO FRAME BUFFER OBJECT	//////////////////////
	//setup offscreen rendering fbo
	glGenFramebuffers(1, &fboID);
	glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	//generate one colour attachment and one depth attachment texture
	glGenTextures(1, &normalTextureID);
	glGenTextures(1, &depthTextureID);

	//bind colour attachment to texture unit 1
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, normalTextureID);

	//set texture parameters
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, RTT_WIDTH, RTT_HEIGHT, 0, GL_BGRA, GL_FLOAT, NULL);

	//bind depth attachment to texture unit 3
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, depthTextureID);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, RTT_WIDTH, RTT_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

	//bind the colour and depth attachments to FBO
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normalTextureID, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, depthTextureID, 0);

	//check FBO completeness status
	GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if(status==GL_FRAMEBUFFER_COMPLETE) {
		printf("FBO setup succeeded.");
	} else {
		printf("Error in FBO setup.");
	}

	//setup filtering fbo
	glGenFramebuffers(1,&filterFBOID);
	glBindFramebuffer(GL_FRAMEBUFFER,filterFBOID);

	//set two colour attachment textures for filtering
	glGenTextures(2, blurTexID);
	for(int i=0;i<2;i++) {
		glActiveTexture(GL_TEXTURE4+i);
		glBindTexture(GL_TEXTURE_2D, blurTexID[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,RTT_WIDTH,RTT_HEIGHT,0,GL_RGBA,GL_FLOAT,NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+i,GL_TEXTURE_2D,blurTexID[i],0);
	}

	//check the framebuffer completeness status
	status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if(status == GL_FRAMEBUFFER_COMPLETE) {
		cout<<"\nFiltering FBO setup successful."<<endl;
	} else {
		cout<<"Problem in Filtering FBO setup."<<endl;
	}

	//bind texture unit 0 as active texture since it will be used for loading 
	//of model textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind the FBO 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

///////////// FIN FRAME BUFFER OBJECT	//////////////////////
}

void ALCHEMY_AO_Init(){
	//segundo paso SSAO shader
	AlchemyAO = new Shaders();
	if(AlchemyAO->isSuported()){
		AlchemyAO->Init("src/Shader/AlchemyAO/AlchemyAO.vsh","src/Shader/AlchemyAO/AlchemyAO.fsh");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	AlchemyAO->Enable();
		
//		AlchemyAO->setUniform("z_far", 1000.0f);
		AlchemyAO->setUniform("tc_window", glm::vec2(float(RTT_WIDTH), float(RTT_HEIGHT) ));
		AlchemyAO->setUniform("projection_matrix", P);
		AlchemyAO->setUniform("view_projection_matrix", P * V);
//		AlchemyAO->setUniform("wc_camera_eye_position", glm::vec3(Cam.m_x, Cam.m_y, Cam.m_z));
		AlchemyAO->setUniform("normalTex", 1);
		AlchemyAO->setUniform("noiseTex", 2);
		AlchemyAO->setUniform("depthTex", 3);

		AlchemyAO->setUniform("NUM_SPIRAL_TURNS", NUM_SPIRAL_TURNS);
		AlchemyAO->setUniform("NUM_SAMPLES", NUM_SAMPLES);
		AlchemyAO->setUniform("aoRad", aoRad);
		AlchemyAO->setUniform("aoBias", aoBias);
		AlchemyAO->setUniform("VerAO", ActivarVerAlchemyAO);
			
	AlchemyAO->Disable();
}

void SAO_Init(){
	//segundo paso SSAO shader
	SAO = new Shaders();
	if(SAO->isSuported()){
		SAO->Init("src/Shader/SAO/SAO.vsh","src/Shader/SAO/SAO.fsh");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	SAO->Enable();
		
		SAO->setUniform("tc_window", glm::vec2(float(RTT_WIDTH), float(RTT_HEIGHT) ));
		SAO->setUniform("projection_matrix", P);
		SAO->setUniform("view_projection_matrix", P * V);
//		SAO->setUniform("wc_camera_eye_position", glm::vec3(Cam.m_x, Cam.m_y, Cam.m_z));
		SAO->setUniform("normalTex", 1);
		SAO->setUniform("noiseTex", 2);
		SAO->setUniform("depthTex", 3);

		SAO->setUniform("NUM_SAMPLES", SAONUM_SAMPLES);
		SAO->setUniform("NUM_SPIRAL_TURNS", SAONUM_SPIRAL_TURNS);
		SAO->setUniform("VARIATION", SAOVARIATION);
//		SAO->setUniform("intensity", SAOintensity);
		SAO->setUniform("sampleRadiusWS", SAOradius);
		SAO->setUniform("bias", SAObias);
		SAO->setUniform("zNear", SAOzNear);
		SAO->setUniform("zFar", SAOzFar);
		SAO->setUniform("projScale", SAOprojScale);
			
	SAO->Disable();

}

void UnrealAO_Init(){
	//segundo paso SSAO shader
	UnrealEngineAO = new Shaders();
	if(UnrealEngineAO->isSuported()){
		UnrealEngineAO->Init("src/Shader/UnrealEngineAO/UnrealEngineAO.vsh","src/Shader/UnrealEngineAO/UnrealEngineAO.fsh");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	//valores iniciales
	UnrealEngineAO->Enable();
		
		UnrealEngineAO->setUniform("tc_window", glm::vec2(float(RTT_WIDTH), float(RTT_HEIGHT) ));
		UnrealEngineAO->setUniform("projection_matrix", P);
//		UnrealEngineAO->setUniform("view_projection_matrix", P * V);
		UnrealEngineAO->setUniform("normalTex", 1);
		UnrealEngineAO->setUniform("noiseTex", 2);
		UnrealEngineAO->setUniform("depthTex", 3);

		UnrealEngineAO->setUniform("NUM_SAMPLE_DIRECTIONS", NUM_SAMPLE_DIRECTIONS);
		UnrealEngineAO->setUniform("NUM_SAMPLE_STEPS", NUM_SAMPLE_STEPS);
		//UnrealEngineAO->setUniform("APPLY_ATTENUATION", APPLY_ATTENUATION);
		//UnrealEngineAO->setUniform("USE_ACTUAL_NORMALS", USE_ACTUAL_NORMALS);
		//UnrealEngineAO->setUniform("uFOV", uFOV);
		UnrealEngineAO->setUniform("uSampleRadius", uSampleRadius);
		UnrealEngineAO->setUniform("uAngleBias", uAngleBias);
		UnrealEngineAO->setUniform("uIntensity", uIntensity);
			
	UnrealEngineAO->Disable();
}

void MSSAO_Init(){
	//final shader
	ShaderFinalMSSAO = new Shaders();
	if(ShaderFinalMSSAO->isSuported()){
		ShaderFinalMSSAO->Init("src/Shader/MSSAO/Passthrough.vert","src/Shader/MSSAO/final.frag");
	}else{
		cout<<"No soporta el shader"<<endl;
		exit(0);
	}
	ShaderFinalMSSAO->Enable();
		ShaderFinalMSSAO->setUniform("intensidadTex1", intensidadTex1);
		ShaderFinalMSSAO->setUniform("intensidadTex2", intensidadTex2);
		ShaderFinalMSSAO->setUniform("intensidadTex3", intensidadTex3);
		ShaderFinalMSSAO->setUniform("intensidadTex4", intensidadTex4);
		ShaderFinalMSSAO->setUniform("intensidadTex5", intensidadTex5);
	ShaderFinalMSSAO->Disable();

	cout<<"\nMSSAO"<<endl;

	///////////// INICIO FRAME BUFFER OBJECT	//////////////////////
	//setup offscreen rendering fbo
	glGenFramebuffers(LEVEL_COUNT, frameBufs);
	glGenFramebuffers(LEVEL_COUNT,filterframeBufs);
	//glBindFramebuffer(GL_FRAMEBUFFER, fboID);

	//generate one colour attachment and one depth attachment texture
	glGenTextures(LEVEL_COUNT, posTex);
	glGenTextures(LEVEL_COUNT, normTex);
	glGenTextures(LEVEL_COUNT*2, aoTexBlur);

	int contador = 0;
	int contador1 = 0;
	int MSSAO_RTT_WIDTH = RTT_WIDTH;
	int MSSAO_RTT_HEIGHT = RTT_HEIGHT;

	for (int i = 0; i < LEVEL_COUNT; ++i)																		//Enlazar cada una de las 5 textura para definir sus propiedades
	{
		//bind colour attachment to texture unit 1
		glActiveTexture(GL_TEXTURE6+contador);
		contador++;
		glBindTexture(GL_TEXTURE_2D, normTex[i]);

		//set texture parameters
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, MSSAO_RTT_WIDTH, MSSAO_RTT_HEIGHT, 0, GL_BGRA, GL_FLOAT, NULL);

		//bind depth attachment to texture unit 3
		glActiveTexture(GL_TEXTURE6+contador);
		contador++;
		glBindTexture(GL_TEXTURE_2D, posTex[i]);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, MSSAO_RTT_WIDTH, MSSAO_RTT_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);

		//bind the colour and depth attachments to FBO
		glBindFramebuffer(GL_FRAMEBUFFER, frameBufs[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, normTex[i], 0);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D, posTex[i], 0);

		//check FBO completeness status
		GLuint status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

		if(status==GL_FRAMEBUFFER_COMPLETE) {
			printf("FBO setup succeeded.");
		} else {
			printf("Error in FBO setup.");
		}

		//setup filtering fbo
		glBindFramebuffer(GL_FRAMEBUFFER,filterframeBufs[i]);

		//set two colour attachment textures for filtering
		for(int j=0;j<2;j++) {
			glActiveTexture(GL_TEXTURE6+contador);
			contador++;
			glBindTexture(GL_TEXTURE_2D, aoTexBlur[contador1]);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
			glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA32F,MSSAO_RTT_WIDTH,MSSAO_RTT_HEIGHT,0,GL_RGBA,GL_FLOAT,NULL);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+j,GL_TEXTURE_2D,aoTexBlur[contador1],0);
			contador1++;
		}

		//check the framebuffer completeness status
		status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
		if(status == GL_FRAMEBUFFER_COMPLETE) {
			cout<<"\nFiltering FBO setup successful."<<endl;
		} else {
			cout<<"Problem in Filtering FBO setup."<<endl;
		}

		//reduciendo resolucion
		MSSAO_RTT_WIDTH /= 2;
		MSSAO_RTT_HEIGHT /= 2; 
	}

	MSSAO_RTT_WIDTH = RTT_WIDTH;
	MSSAO_RTT_HEIGHT = RTT_HEIGHT;
	//bind texture unit 0 as active texture since it will be used for loading 
	//of model textures
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);

	//unbind the FBO 
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
///////////// FIN FRAME BUFFER OBJECT	//////////////////////
}

void init()
{
	glEnable(GL_DEPTH_TEST);	//ZBuffer
	glEnable(GL_CULL_FACE);		//BACK FACE
	//glEnable(GL_NORMALIZE);	//los vectores normales se escalan a longitud de unidad despues de las transformacione y antes de iluminar
	glEnable(GL_RESCALE_NORMAL);

	initAntTweak();
	initVar();
	glewInit();

	//Inicializando las matrices
	RotMatrix = glm::mat4(1.0f); 
	transMatrix = glm::mat4(1.0f);
	scaleMatrix = glm::mat4(1.0f);
	tempMatrix1 = glm::mat3(1.0f);
	M = glm::mat4(1.0f);	
	//V = glm::lookAt(glm::vec3(0.0f,10.0f,10.0f), glm::vec3(0.0f,0.0f,0.0f), glm::vec3(0.0f,1.0f,0.0f));		
	P = glm::perspective(60.0f, (GLfloat)(WWidth/WHeight), 0.01f, 1000.0f);
	MV = glm::mat4(1.0f);
	MVP = glm::mat4(1.0f);

	//Load Shaders
	initShaders();
	//cargar objetos
	initObjetos();
	//Iniciar SSAO
	SSAO_Init();
	//Iniciar ALCHEMY AO
	ALCHEMY_AO_Init();
	//Iniciar SCALABLE AMBIENT OBSCURANCE (SAO)
	SAO_Init();
	//Iniciar Unreal Engine AO
	UnrealAO_Init();
	//Iniciar MSSAO
	MSSAO_Init();
	
	ShaderA->Enable();
		ShaderA->setUniform("VerSoloAO", VerSoloAO);
	ShaderA->Disable();
}

//Main Principal
int main(int argc, char *argv[])
{	

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowPosition(0,0);
	//tamaño de la ventana
	glutInitWindowSize(WWidth,WHeight);
	glutCreateWindow("Tesis");
	//glutGameModeString( "1600×900:32@75" );
    //glutEnterGameMode();
	//glutFullScreen();

	//output hardware information
	cout<<"\tUsing GLEW "<<glewGetString(GLEW_VERSION)<<endl;
	cout<<"\tVendor: "<<glGetString (GL_VENDOR)<<endl;
	cout<<"\tRenderer: "<<glGetString (GL_RENDERER)<<endl;
	cout<<"\tVersion: "<<glGetString (GL_VERSION)<<endl;
	cout<<"\tGLSL: "<<glGetString (GL_SHADING_LANGUAGE_VERSION)<<endl;

	init();

	glutDisplayFunc(renderScene); //Manejo del render
	glutKeyboardFunc(keyboard);  //Manejo del teclado
	glutKeyboardUpFunc(KeyboardUp);
	glutMouseFunc(left_click);
	glutMotionFunc(movimiento);
	glutPassiveMotionFunc(movimiento);
	glutReshapeFunc(reshape);
	glutTimerFunc(1, Timer, 0);

	glutMainLoop();
}